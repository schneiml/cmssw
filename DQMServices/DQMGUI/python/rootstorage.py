import threading
import sqlite3
import struct
import zlib
import time
import mmap
import re

from concurrent.futures import ProcessPoolExecutor
from collections import namedtuple, defaultdict
from functools import lru_cache

from  DQMServices.DQMGUI import nanoroot

DBNAME = "directory.sqlite"

DBSCHEMA = """
BEGIN;
CREATE TABLE IF NOT EXISTS samples(dataset, run, lumi, filename, menamesid, meoffsetsid, problems);
CREATE INDEX IF NOT EXISTS samplelookup ON samples(dataset, run, lumi);
CREATE UNIQUE INDEX IF NOT EXISTS uniquesamples on samples(dataset, run, lumi, filename);
CREATE TABLE IF NOT EXISTS menames(menamesid INTEGER PRIMARY KEY, menameblob);
CREATE UNIQUE INDEX IF NOT EXISTS menamesdedup ON menames(menameblob);
CREATE TABLE IF NOT EXISTS meoffsets(meoffsetsid INTEGER PRIMARY KEY, meoffsetsblob);
COMMIT;
"""
with sqlite3.connect(DBNAME) as db:
    db.executescript(DBSCHEMA)

# This is used for indexing.
pool = ProcessPoolExecutor(max_workers=10)
inprogress = dict()

# Some types, related to the DB schema.
Sample = namedtuple("Sample", ["dataset", "run", "lumi", "filename", "menamesid", "meoffsetsid"],
                   defaults = [None,      None,   0,     None,      None,        None])
EfficiencyFlag = namedtuple("EfficiencyFlag", ["name"])
ScalarValue = namedtuple("ScalarValue", ["name", "value", "type"])
QTest = namedtuple("QTest", ["name", "qtestname", "status", "result", "algorithm", "message"])

# don't import these (kown obsolete/broken stuff)
# The notorious SiStrip bad component workflow creates are varying number of MEs
# for each run. We just hardcode-ban them here to help the deduplication
blacklist = re.compile(b"By Lumi Section |/Reference/|BadModuleList")

# This class represents a ME in storage. It needs to be very small, since we 
# will store billions of these in DB. For example, it does not know it's name.
# It does store the offsets into the ROOT file and everything else needed to
# read the data from the ROOT file.
class MEInfo:
    idtotype = {
      # these match the DQMIO encoding where possible. But they don't really need to.
      0: b"Int",
      1: b"Float",
      2: b"String",
      3: b"TH1F",
      4: b"TH1S",
      5: b"TH1D",
      6: b"TH2F",
      7: b"TH2S",
      8: b"TH2D",
      9: b"TH3F",
      10: b"TProfile",
      11: b"TProfile2D",
      20: b"Flag",
      21: b"QTest",
      22: b"XMLString", # For string type in TDirectory
    }
    typetoid = {v: k for k, v in idtotype.items()}

    # These are used to store the MEInfo into a blob. The scalar
    # version stores the value directly. They are all the
    # same size to allow direct indexing.
    normalformat = struct.Struct("<qiihh")
    # This is a bit of a hack: the deltaencode below needs all topmost bits to be unused.
    # so we spread the double and int64 values for scalars with a bit of padding to keep them free.
    scalarformat = struct.Struct("<xBBBBBBxxBBxxxxxHxx") 
    intformat    = struct.Struct("<q")
    floatformat  = struct.Struct("<d")
    
    @staticmethod
    def listtoblob(meinfos):
        # For the meinfos, Zlib compression is not very effective: there is
        # little repetition for the LZ77 and the Huffmann coder struggles with the
        # large numbers.
        # But, since the list is roughly increasing in order, delta coding makes most
        # values small. Then, the Huffmann coder in Zlib compresses well.
        # Decreases output size about 4x.
        words = MEInfo.normalformat
        def deltacode(a):
            prev = [0,0,0,0,0]
            for x in a:
                new = words.unpack(x)
                yield words.pack(*[a-b for a, b in zip(new, prev)])
                prev = new
        delta = deltacode(i.pack() for i in meinfos)
        buf = b''.join(delta)
        infoblob = zlib.compress(buf)
        print (f"MEInfo: compression {len(buf)/len(infoblob)}, total {len(buf)}")
        return infoblob
    
    @staticmethod
    def blobtopacked(infoblob):
        words = MEInfo.normalformat
        def deltadecode(d):
            prev = [0,0,0,0,0]
            for x in d:
                new = [a+b for a, b in zip(prev, words.unpack(x))]
                yield words.pack(*new)
                prev = new
        buf = zlib.decompress(infoblob)
        packed = deltadecode(buf[i:i+words.size] for i in range(0, len(buf), words.size))
        return list(packed)
    
    @staticmethod
    def blobtolist(infoblob):
        packed = MEInfo.blobtopacked(infoblob)
        return [MEInfo.unpack(x) for x in packed]
    
    def __init__(self, metype, seekkey = 0, offset = 0, size = -1, value = None, qteststatus = 0):
        self.metype = metype
        self.seekkey = seekkey
        self.offset = offset
        self.size = size
        self.value = value
        self.qteststatus = qteststatus
        
    def __repr__(self):
        return (f"MEInfo(metype={repr(self.metype)}" +
            f"{', seekkey=%d' % self.seekkey if self.seekkey else ''}" +
            f"{', offset=%d' % self.offset if self.offset else ''}" +    
            f"{', size=%d' % self.size if self.size > 0 else ''}" +
            f"{', value=%s' % repr(self.value) if self.value else ''}" +
            f"{', qteststatus=%d' % self.qteststatus if self.qteststatus else ''}" +
            ")")
    
    def __lt__(self, other):
        return (self.metype, self.seekkey, self.offset) <  (other.metype, other.seekkey, other.offset)
    
    def pack(self):
        if self.metype == b'Int':
            buf = MEInfo.intformat.pack(self.value)
            return MEInfo.scalarformat.pack(*buf, MEInfo.typetoid[self.metype])
        if self.metype == b'Float':
            buf = MEInfo.floatformat.pack(self.value)
            return MEInfo.scalarformat.pack(*buf, MEInfo.typetoid[self.metype])
        return MEInfo.normalformat.pack(self.seekkey, self.offset, self.size, 
                                        MEInfo.typetoid[self.metype], self.qteststatus)
    @staticmethod
    def unpack(buf):
        k, o, s, t, st = MEInfo.normalformat.unpack(buf)
        metype = MEInfo.idtotype[t]
        if metype == b'Int':
            buf = MEInfo.scalarformat.unpack(buf)
            v, = MEInfo.intformat.unpack(bytes(buf[:-1])) # last is metype again
            return MEInfo(metype, value = v)
        if metype == b'Float':
            buf = MEInfo.scalarformat.unpack(buf)
            v, = MEInfo.floatformat.unpack(bytes(buf[:-1])) # last is metype again
            return MEInfo(metype, value = v)
        return MEInfo(metype, seekkey = k, offset = o, size = s, qteststatus = st)
    
    def read(self, rootfile):
        if self.value != None:
            return ScalarValue(b'', self.value, b'') # TODO: do sth. better.
        key = nanoroot.TKey(rootfile, self.seekkey)
        data = key.objdata()
        if self.metype == b'QTest':
            # TODO: this won't work for DQMIO, but we don't have QTests there anyways...
            return parsestringentry(key.objname())
        if self.metype == b'XMLString':
            return parsestringentry(key.objname())
        if self.offset == 0 and self.size == -1:
            obj = data
        else:
            obj = data[self.offset : self.offset + self.size]
        if self.metype == b'String':
            s = nanoroot.String.unpack(obj, 0, len(obj), None)
            return ScalarValue(b'', s, b's')
        # else
        classversion = 3 #TODO: we need to have a better guess here...
        # metype doubles as root class name here.
        return nanoroot.TBufferFile(obj, self.metype, classversion) 

def parsestringentry(val):
    # non-object data is stored in fake-XML strings in the TDirectory.
    # This decodes these strings into an object of correct type.
    assert val[0] == b'<'[0]
    name = val[1:].split(b'>', 1)[0]
    value = val[1+len(name)+1:].split(b'<', 1)[0]
    if value == b"e=1": # efficiency flag on this ME
        return EfficiencyFlag(name)
    elif len(value) >= 2 and value[1] == b'='[0]:
        return ScalarValue(name, value[2:], value[0:1])
    else: # should be a qtest in this case
        assert value.startswith(b'qr=')
        assert b'.' in name
        mename, qtestname = name.split(b'.', 1)
        parts = value[3:].split(b':', 4)
        assert len(parts) == 5, "Expect 5 parts, not " + repr(parts)
        x, status, result, algorithm, message = parts
        assert x == b'st'
        return QTest(mename, qtestname, status, result, algorithm, message)
        

# SQLite is at heart single-threaded. However, since we expect a read-heavy
# workload, we can work around that by having multiple connections open at the
# same time. 
class DBHandle:
    def __init__(self, cache):
        self.cache = cache
        self.db = None
    def __enter__(self):
        self.cache[0].acquire()
        self.db = self.cache.pop()
        return self.db
    def __exit__(self, type, value, traceback):
        self.cache.append(self.db)
        self.db = None
        self.cache[0].release()
dbcache = [threading.Lock(), sqlite3.connect(DBNAME, check_same_thread=False)]

# TODO: this should go somewhere else, just some glue code for now.
# TODO: this is where Rucio comes in in the future.
def registerfiles(fileurls):
    def tosample(fileurl):
        name = fileurl.split("/")[-1]
        mainpart = name[11:-5] # remove DQM_V00x_R .root
        parts = mainpart.split("__")
        run = int(parts[0])
        dataset = "/" + "/".join(parts[1:])
        lumi = 0
        return Sample(dataset, run, lumi, fileurl)
    samples = [tosample(f) for f in fileurls]
    registersamples(samples)

def registersamples(samples):
    with DBHandle(dbcache) as db:
        db.execute("BEGIN;")
        db.executemany("INSERT OR REPLACE INTO samples(dataset, run, lumi, filename) VALUES(?, ?, ?, ?);", 
                   [(sample.dataset, sample.run, sample.lumi, sample.filename) for sample in samples])
        db.execute("COMMIT;")

# Index TDirectory data. This lauches the actual indexer in a process pool.
# This will usually happen on-demand.
def importsample(sample):
    if sample in inprogress:
        # TODO: clean up done jobs here, but that is hard to get thread safe.
        # result() should block if it is not done.
        nameblob, offsetblob, problem = inprogress[sample].result()
        storemelist(sample, nameblob, offsetblob, problem)
    else:     
        # this is not 100% thread safe but it is not terrible to have multiple jobs for the same sample.
        inprogress[sample] = pool.submit(importsampleimpl, sample)
        return importsample(sample) # this time it should hit the other branch.

def importsampleimpl(sample):
    # Remove the folder strucutre that CMSSW adds.
    # TODO: Check run numbers here?
    def dqmnormalize(parts):
        if len(parts) < 5 or parts[4] != b'Run summary':
            return b'<broken>' + b'/'.join(parts) + b'/'
        else:
            return b'/'.join((parts[3],) + (parts[5:]) + (b'',))
            
    # Only import these types.
    def dqmclasses(name):
        return name in {
            b'TH1D',
            b'TH1F',
            b'TH1S',
            b'TH2D',
            b'TH2F',
            b'TH2S',
            b'TH3F',
            b'TObjString',
            b'TProfile',
            b'TProfile2D',
        }
    melist = []
    try:
        f, mm = openfile(sample.filename, pagesize=2**30) # Just preload the full file
        tf = nanoroot.TFile(mm, normalize=dqmnormalize, classes=dqmclasses)
        melist = list(recursivelist(tf))
        print("meslist: ", len(melist))
        error = "Problems on import" if tf.error else None
    except Exception as e:
        melist = []
        error = "Exception on import: " + repr(e)
        raise e # comment out to actually ignore and continue
    finally:
        f.close(), mm.close()
    nameblob, offsetblob = melisttoblob(melist)
    return (nameblob, offsetblob, error)

def recursivelist(tfile):
    for path, name, cls, offset in tfile.fulllist():
        if blacklist.search(path):
            continue
        if cls == b'TObjString':
            thing = parsestringentry(name)
            if isinstance(thing, EfficiencyFlag): # efficiency flag on this ME
                yield (path + thing.name + b'\0e=1', MEInfo(b'Flag'))
            elif isinstance(thing, ScalarValue):
                # scalar ME.
                if thing.type == b'i':
                    yield (path + thing.name, MEInfo(b'Int', value = int(thing.value.decode("ascii"))))
                elif thing.type == b'f':
                    yield (path + thing.name, MEInfo(b'Float', value = float(thing.value.decode("ascii"))))
                else:
                    yield (path + thing.name, MEInfo(b'XMLString', offset))
            else:
                # QTest. Only save mename and qtestname, values need to be fetched later.
                # Separate QTest name with \0 to prevent collisions with ME names.
                yield (path + thing.name + b'\0.' + thing.qtestname, 
                       MEInfo(b'QTest', offset, qteststatus=int(thing.status.decode("ascii"))))
        else:
            # path position
            yield (path + name, MEInfo(cls, offset))

# Import DQMIO data. This is much harder to do on demand, unless we know which
# Sample are in the file from some other source.
def importdqmio(dataset, filename):
    from DQMServices.DQMGUI.nanoroot import Int32, Int64, Float64, String, ObjectBlob
    dqmioschema = {
        b'Indices': {b'Run': Int32, b'Lumi': Int32, b'Type': Int32,
                     b'FirstIndex': Int64, b'LastIndex': Int64},
        b'Ints':   {b'FullName': String, b'Value': Int64},
        b'Floats': {b'FullName': String, b'Value': Float64},
        b'Strings':{b'FullName': String, b'Value': ObjectBlob},
        b'TH1Fs':  {b'FullName': String, b'Value': ObjectBlob},
        b'TH1Ss':  {b'FullName': String, b'Value': ObjectBlob},
        b'TH1Ds':  {b'FullName': String, b'Value': ObjectBlob},
        b'TH2Fs':  {b'FullName': String, b'Value': ObjectBlob},
        b'TH2Ss':  {b'FullName': String, b'Value': ObjectBlob},
        b'TH2Ds':  {b'FullName': String, b'Value': ObjectBlob},
        b'TProfiles':   {b'FullName': String, b'Value': ObjectBlob},
        b'TProfile2Ds': {b'FullName': String, b'Value': ObjectBlob},
    }
    treenames = {
      0: b"Ints",
      1: b"Floats",
      2: b"Strings",
      3: b"TH1Fs",
      4: b"TH1Ss",
      5: b"TH1Ds",
      6: b"TH2Fs",
      7: b"TH2Ss",
      8: b"TH2Ds",
      9: b"TH3Fs",
      10: b"TProfiles",
      11: b"TProfile2Ds",
    }

    with open(filename, 'rb') as f:
        with mmap.mmap(f.fileno(), 0, prot=mmap.PROT_READ) as mm:
            trees = nanoroot.TTreeFile(nanoroot.TFile(mm), dqmioschema)

    IndexEntry = namedtuple("IndexEntry", [name.decode("utf-8") for name in trees[b'Indices'].branchnames])
    idxtree = trees[b'Indices']
    entries = [IndexEntry(*x) for x in idxtree]
            
    def createinfo(value, typeid):
        metype = MEInfo.idtotype[typeid]
        if metype in (b'Int', b'Float'):
            return MEInfo(metype, value = value)
        else:
            # value is ObjectBlob.Range
            return MEInfo(metype, value.fSeekKey, value.start, value.end - value.start)

    infos = defaultdict(list)
    for e in entries:
        tree = trees[treenames[e.Type]]
        names = tree.branches[b'FullName'][e.FirstIndex : e.LastIndex+1]
        values = [createinfo(v, e.Type) for v in tree.branches[b'Value'][e.FirstIndex : e.LastIndex+1]]
        infos[(e.Run, e.Lumi)] += list(zip(names, values))
    out = []
    for (run, lumi), melist in infos.items():
        if lumi > 0: continue # Only run data for this test.
        melist.sort()
        nameblob, infoblob = melisttoblob(melist)
        out.append((Sample(dataset, run, lumi, filename), nameblob, infoblob))
    return out

def tryimportdqmio(dataset, filename):
    try:
        return importdqmio(dataset, filename)
    except Exception as e:
        print("Failed to import DQMIO file ", filename, e)
        return []

def melisttoblob(melist):
    melist.sort()
    buf = b'\n'.join(key for key, _ in melist)
    nameblob = zlib.compress(buf)
    print (f"Compression {len(buf)/len(nameblob)}, total {len(buf)}")
    infos = [info for _, info in melist]
    infoblob = MEInfo.listtoblob(infos)
    return nameblob, infoblob

def meoffsetsfromblob(offsetblob):
    return MEInfo.blobtolist(offsetblob)

def melistfromblob(namesblob):
    return zlib.decompress(namesblob).splitlines()

def storemelist(sample, nameblob, offsetblob, problems = None):
    # TODO: Sqlite does not like MT-writing. Use a RWLock or sth.?
    # TODO: make this an upsert if the sample already exists.
    with DBHandle(dbcache) as db:
        db.execute("BEGIN;")
        cur = db.execute("SELECT menamesid, meoffsetsid, problems FROM samples WHERE (dataset, run, lumi, filename) == (?, ?, ?, ?);", 
                   (sample.dataset, sample.run, sample.lumi, sample.filename))
        menamesid, meoffsetsid, existingproblems = list(cur)[0]
        if menamesid and  meoffsetsid and existingproblems == problems:
            print(f"Sample {sample} exists already, ignored.")
            return
        db.execute("INSERT OR IGNORE INTO menames(menameblob) VALUES(?);", (nameblob,))
        cur = db.execute("SELECT menamesid FROM menames WHERE menameblob = ?;",  (nameblob,))
        menamesid = list(cur)[0][0]
        db.execute("INSERT INTO meoffsets(meoffsetsblob) VALUES(?);", (offsetblob,))
        cur = db.execute("SELECT last_insert_rowid();")
        meoffsetsid = list(cur)[0][0]
        db.execute("UPDATE samples SET (menamesid, meoffsetsid, problems) = (?, ?, ?) WHERE (dataset, run, lumi, filename) == (?, ?, ?, ?);", 
                   (menamesid, meoffsetsid, problems, sample.dataset, sample.run, sample.lumi, sample.filename))
        db.execute("COMMIT;")


# This is the main entry point. Given a (partial) Sample tuple and a full name,
# return data for this ME.
# The data will be a list of the main object blob and (potentially) more 
# QTest/Flag/ScalarValue objects.
def readme(sample, fullname):
    fullsample = queryforsample(sample)
    mes = melistfromid(fullsample.menamesid)
    offsets = meoffsetsfromid(fullsample.meoffsetsid)
    keys = locateme(mes, offsets, fullname)
    with FileHandle(fullsample.filename, filecache) as f:
        return [info.read(f[1]) for name, info in keys]
    
# Collect sample info from DB.
# All DB accesses are per-sample and cached.
@lru_cache(maxsize=10)
def queryforsample(sample):
    with DBHandle(dbcache) as db:
        cur = db.execute("SELECT filename, menamesid, meoffsetsid FROM samples WHERE (dataset, run, lumi) == (?, ?, ?);", 
                         (sample.dataset, sample.run, sample.lumi))
        file, menamesid, meoffsetsid = list(cur)[0]
        sample = Sample(sample.dataset, sample.run, sample.lumi, file, menamesid, meoffsetsid)
    if file and menamesid == None or meoffsetsid == None:
        importsample(sample)
        return queryforsample(sample) # retry
    return sample

# These are primarily to have a place for the caches.
# The menames cache should have a high hit rate.
@lru_cache(maxsize=10)
def melistfromid(menamesid):
    with DBHandle(dbcache) as db:
        cur = db.execute("SELECT menameblob FROM menames WHERE menamesid = ?;", (menamesid,))
        blob = list(cur)[0][0]
        return melistfromblob(blob)
    
@lru_cache(maxsize=10)
def meoffsetsfromid(meoffsetsid):
    with DBHandle(dbcache) as db:
        cur = db.execute("SELECT meoffsetsblob FROM meoffsets WHERE meoffsetsid = ?;", (meoffsetsid,))
        blob = list(cur)[0][0]
        return meoffsetsfromblob(blob)

# search for a name in the list of ME names.
# Uses binary search with bound() below.
def locateme(mes, offsets, fullname):
    # This uses a simple order by fullnames, sorted in melisttoblob.
    # This is different from the (path, name) order in DQMIO FullName column.
    # But fine, since we make the list ourselves.
    def access(i):
        return mes[i]
    idx = bound(access, fullname, 0, len(mes))
    while idx < len(mes) and access(idx) == fullname:
        offset = offsets[idx]
        # Attach the offset to the tuple. A bit messy since tuples are immutable...
        yield (mes[idx], offset)
        idx += 1

# Return the index of the first element that is not smaller than key.
# lower is the index of the first element to consider.
# upper is the first element known to be not smaller than key. Must not be touched.
# access is a function used to access elements, and lessthan the comparison functor.
def bound(access, key, lower, upper):
    # nothing more to do, upper is always a valid return value.
    if lower == upper: return upper
    # mid will be smaller than upper, but may be equal to lower.
    mid = lower + int((upper-lower)/2)
    # therefore, it is always save to read a(mid), and we have not done so before.
    if access(mid) < key: 
        # if it is too small, exclude it from the range.
        return bound(access, key, mid+1, upper)
    else:
        # if it is a valid return value, reduce the range and never touch it again.
        return bound(access, key, lower, mid)


filecache = defaultdict(list)
def openfile(filename, pagesize = 2**20):
    # maybe add more options here
    if filename.startswith("root:"):
        xf = nanoroot.XRDFile(filename)
        cf = nanoroot.CachedFile(xf, pagesize=pagesize)
        file = (xf, cf)
    else: 
        f = open(filename, 'rb')
        mm = mmap.mmap(f.fileno(), 0, prot=mmap.PROT_READ)
        file = (f, mm)
    return file

class FileHandle:
    def __init__(self, filename, cache):
        self.cache = cache
        self.filename = filename
        self.file = None
    def __enter__(self):
        try:
            while not self.file:
                lastused, self.file = self.cache[self.filename].pop()
                if time.time() - lastused > 60:
                    # too old, don't use.
                    self.file[0].close()
                    self.file[1].close()
                    self.file = None
        except IndexError:
            # no open files left
            self.file = openfile(self.filename)
        return self.file
    def __exit__(self, type, value, traceback):
        self.cache[self.filename].append((time.time(), self.file))

# Samples search API
def searchsamples(datasetre = None, runre = None):
    dsmatch = re.compile(datasetre) if datasetre else None
    runmatch = re.compile(runre) if runre else None
    out = []
    with DBHandle(dbcache) as db:
        # TODO: use SQLite with regex extension, pass down re's here.
        cur = db.execute("SELECT DISTINCT dataset, run FROM samples ORDER BY dataset, run;")
        for dataset, run in cur:
            if (not dsmatch or dsmatch.search(dataset)) and (not runmatch or runmatch.search(str(run))):
                out.append(Sample(dataset, run, 0))
        return out

# list API, should be sort-of compatible with old GUI behaviour with recursive = False
def listmes(sample, path, match=None, recursive=True):
    s = queryforsample(sample)
    mes = melistfromid(s.menamesid)
    if match:
        exp = re.compile(match)
        cand = [x for x in mes if x.startswith(path) and exp.search(x)]
    else:
        cand = [x for x in mes if x.startswith(path)]
    if recursive:
        return set(cand)
    else:
        # return only next-level names
        def relativize(subpath):
            tail = subpath[len(path):]
            if b'/' in tail:
                tail = tail.split(b'/')[0] + b'/'
            if b'\0' in tail: # QTest with secondary name
                tail = tail.split(b'\0')[0] 
            return tail
        return set(relativize(x) for x in cand)
        


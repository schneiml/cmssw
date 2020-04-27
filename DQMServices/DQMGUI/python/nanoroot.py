import re
import zlib
import struct
from collections import namedtuple

# If uproot is micro-Python ROOT, this is Nano ROOT.
# These classes allow reading the ROOT contianer structures (TFile, TDirectory, TKey),
# but not actually decoding ROOT serialized objects.
# The reason to use this rather than uproot is much higher performance on TDirectory's,
# as well as fewer dependencies.
# Everything is designed around a buffer like it is returned by mmap.mmap, that supports
# indexing to get bytes. Reading from the bufer is rather lazy and copying data is
# avoided as long as possible.

class TFile:
    # Structure from here: https://root.cern.ch/doc/master/classTFile.html
    Fields = namedtuple("TFileFields", ["root", "fVersion", "fBEGIN", "fEND", "fSeekFree", "fNbytesFree", "nfree", "fNbytesName", "fUnits", "fCompress", "fSeekInfo", "fNbytesInfo", "fUUID"])
    structure = struct.Struct(">4sIIIIIIIbIIII")

    
    # These two are default transforms that can be overwritten.
    topath = lambda parts: b'/'.join(parts) + b'/'
    allclasses = lambda name: True

    # The TFile datastructure is pretty boring, the only thing we really need
    # is the address of the first TKey, which is actually hardcode to 100...
    # But this class also provides some caching to make computing full paths
    # fast, and the `fulllist` method that lists all objects using this.
    # Its behaviour can be adjusted with the `normalize` and `classes` 
    # callback functions.
    def __init__(self, buf, normalize = topath, classes = allclasses):
        self.buf = buf
        self.normalize = normalize
        self.classes = classes
        self.fields = TFile.Fields(*TFile.structure.unpack(
            self.buf[0 : TFile.structure.size]))
        assert self.fields.root == b'root'
        self.normalizedcache = dict()
        self.dircache = dict()
        self.dircache[0] = ()
        self.error = False
        
    def __repr__(self):
        return f"TFile({self.buf}, fields = {self.fields})"

    # First TKey in the file. They are sequential, use `next` on the key to
    # get the next key.
    def first(self):
        return TKey(self.buf, self.fields.fBEGIN, self.end())

    def end(self):
        if len(self.buf) < self.fields.fEND:
            self.error = True
            print(f"TFile corrupted, fEND ({self.fields.fEND}) behind end-of-file ({len(self.buf)})")
            return len(self.buf)
        return self.fields.fEND

    # Not that useful, but one could build a small file with only the streamers using this.
    def streamerinfo(self):
        return self.buf[tfile.fields.fSeekInfo : tfile.fields.fSeekInfo + tfile.fields.fNbytesInfo]
    
    def fullname(self, key):
        return (self._normalized(key.fields.fSeekPdir), key.objname())
    
    def _fullname(self, fSeekKey):
        if fSeekKey in self.dircache:
            return self.dircache[fSeekKey]
        k = TKey(self.buf, fSeekKey)
        p = k.fields.fSeekPdir
        res = self._fullname(p) + (k.objname(),)
        self.dircache[fSeekKey] = res
        return res
    
    def _normalized(self, fSeekKey):
        if fSeekKey in self.normalizedcache:
            return self.normalizedcache[fSeekKey]
        parts = self._fullname(fSeekKey)
        res = self.normalize(parts)
        self.normalizedcache[fSeekKey] = res
        return res
    
    # Returns a generator producing (path, name, class, offset) tuples.
    # The paths are normalized with the `normalize` callback, the classes 
    # filtered with the `classes` callback.
    def fulllist(self):
        x = self.first()
        while x:
            c = x.classname()
            if self.classes(c):
                yield (*self.fullname(x), c, x.fSeekKey)
            n = x.next()
            if x.error:
                self.error = True
            x = n

class TKey:
    # Structure also here: https://root.cern.ch/doc/master/classTFile.html
    Fields = namedtuple("TKeyFields", ["fNbytes", "fVersion", "fObjLen", "fDatime", "fKeyLen", "fCycle", "fSeekKey", "fSeekPdir"])
    structure = struct.Struct(">iHIIHHII")
    sizefield = struct.Struct(">i")
    
    # In this case of curruption, we search for the next known class name and try it there.
    # This should never be used in the normal case.
    resync = re.compile(b'TDirectory|TH[123][DFSI]|TProfile|TProfile2D')
    
    # Decode key at offset `fSeekKey` in `buf`. `end` can be the file end 
    # address if it is less then the buffer end.
    def __init__(self, buf, fSeekKey, end = None):
        self.buf = buf
        self.end = end if end != None else len(buf)
        self.fSeekKey = fSeekKey
        self.fields = TKey.Fields(*TKey.structure.unpack(
            self.buf[self.fSeekKey : self.fSeekKey + TKey.structure.size]))
        assert self.fields.fSeekKey == self.fSeekKey, f"{self} is corrupted!"
        self.error = False
        
    def __repr__(self):
        return f"TKey({self.buf}, {self.fSeekKey}, fields = {self.fields})"
    
    # Read the TKey following this key. According to the documentation these
    # should be one after the other in the file, but in practice there are
    # sometimes gaps (resized/deleted objects?), which are skipped here.
    # If we still fail to read the next item, we try to find the next key
    # by searching for a familiar class name.
    def next(self):
        offset = self.fields.fSeekKey + self.fields.fNbytes
        while (offset+TKey.structure.size) < self.end:
            # It seems that a negative length indicates an unused block of that size. Skip it.
            # The number of such blocks matches nfree in the TFile.
            size, = TKey.sizefield.unpack(self.buf[offset:offset+4])
            if size < 0:
                offset += -size
                continue
            try:
                k = TKey(self.buf, offset, self.end)
                return k
            except AssertionError:
                m = TKey.resync.search(self.buf, offset + TKey.structure.size + 2)
                if not m: break
                newoffset = m.start() - 1 - TKey.structure.size
                print(f"Error: corrupted TKey at {offset}:{repr(self.buf[offset:offset+32])} resyncing (+{newoffset-offset})")
                self.error = True
                offset = newoffset
        return None
        
    def _getstr(self, pos):
        size = self.buf[pos]
        if size == 255: # soultion for when length does not fit one byte
            size, = TKey.sizefield.unpack(self.buf[pos+1:pos+5])
            pos += 4
        nextpos = pos + size + 1
        value = self.buf[pos+1:nextpos]
        return value, nextpos
    
    # Parse the three strings in the TKey (classname, objname, objtitle)
    def names(self):
        pos = self.fSeekKey + TKey.structure.size
        classname, pos = self._getstr(pos)
        objname, pos = self._getstr(pos)
        objtitle, pos = self._getstr(pos)
        return classname, objname, objtitle
    
    def classname(self):
        return self._getstr(self.fSeekKey + TKey.structure.size)[0]
    
    def objname(self):
        # optimized self.names()[1]
        pos = self.fSeekKey + TKey.structure.size
        pos += self.buf[pos] + 1
        if self.buf[pos] == 255:
            size, = TKey.sizefield.unpack(self.buf[pos+1:pos+5])
            pos += 4
            nextpos = pos + size
        else:
            nextpos = pos + self.buf[pos]
        return self.buf[pos+1:nextpos+1]

    def compressed(self):
        return self.fields.fNbytes - self.fields.fKeyLen != self.fields.fObjLen
    
    # Return and potentially decompress object data.
    def objdata(self):
        start = self.fields.fSeekKey + self.fields.fKeyLen
        end = self.fields.fSeekKey + self.fields.fNbytes
        if not self.compressed():
            return self.buf[start:end]
        else:
            comp = self.buf[start:start+2]
            assert comp == b'ZL', "Only Zlib compression supported, not " + repr(comp) 
            # There are a bunch of header bytes, not exactly sure what they mean.
            return zlib.decompress(self.buf[start+9:end])

    # Each key (except the root) has a parent directory.
    # Creates a new TKey pointing there.
    def parent(self):
        if self.fields.fSeekPdir == 0:
            return None
        return TKey(self.buf, self.fields.fSeekPdir, self.end)
    
    
    # Derive the full path of an object by recursing up the parents.
    # slow, primarily for debugging.
    def fullname(self):
        parent = self.parent()
        parentname = parent.fullname() if parent else b''
        return b"%s/%s" % (parentname, self.objname())
        

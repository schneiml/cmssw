import asyncio
from async_lru import alru_cache
from DQMServices.DQMGUI import nanoroot


class IOService:
    BLOCKSIZE = 128*1024 # number of bytes to read at once
    # TODO: not sure if the readahead mechanism is really worth the trouble.
    READAHEAD = 10       # number of blocks to read at once
    CACHEBLOCKS = 1000   # total number of block to keep in cache
    CONNECTIONS = 100    # maximum number of open connections
    OPENTIMEOUT = 5      # maximum time to wait while opening file (seconds)
    MAXOPENTIME = 60     # maximum time that a connection stays open (seconds)

    do_read_ahead = True
    
    
    @classmethod
    async def open_url(cls, url, blockcache=True):
        """
        Create a file handle for local or remote file `url`.

        If `blockcache` is set, this file handle will use the global caching 
        mechanism, else the full file will be read to memory in a local buffer.
        This can be useful for indexing, where we don't want to pollute the
        global cache with lots of blocks that are never read again.
        """
        if blockcache:
            f = BlockCachedFile(url, cls.BLOCKSIZE)
        else:
            f = FullFile(url, timeout = cls.OPENTIMEOUT)
        await f.preload()
        return f
    

    @classmethod
    async def read_ahead(cls, url, firstblock):
        """Internal: trigger reading some extra blocks in background after cache miss."""

        size = await cls.read_len(url)
        for blockid in range(firstblock, firstblock+cls.READAHEAD):
            if size < cls.BLOCKSIZE*blockid:
                break
            await cls.read_block(url, blockid) 
    

    @classmethod
    @alru_cache(maxsize=CACHEBLOCKS)
    async def read_block(cls, url, blockid):
        """ 
        Internal: read a block from a url, creating a connection as needed.

        This method is cached, and that cache is the main block cache.
        """

        # Start reading some blocks. This will read more blocks than we actually
        # need in the background, to pre-populate the cache.
        if cls.do_read_ahead:
            # We need this flag to prevent an infinite recursion once 
            # read_ahead calls read_block again...
            cls.do_read_ahead = False
            asyncio.Task(cls.read_ahead(url, blockid))
        
        file = await cls.__connect(url)
        return await file[blockid*cls.BLOCKSIZE : (blockid+1)*cls.BLOCKSIZE]


    @classmethod
    @alru_cache(maxsize=CONNECTIONS)
    async def read_len(cls, url):
        """Internal: read length of the file at the given url."""
        
        file = await cls.__connect(url)
        return len(file)


    @classmethod
    @alru_cache(maxsize=CONNECTIONS)
    async def __connect(cls, url):
        """Create a pyxrootd.client (via nanoroot) connection to the url."""

        async def closefile():
            # XRD connections tend to break after a while.
            # To prevent this, we preventively close all connections after a certain time.
            await asyncio.sleep(cls.MAXOPENTIME)
            # This is done by removing the file from the cache, GC takes care of the rest.
            cls.__connect.invalidate(cls, url)
        file = await nanoroot.XRDFile().load(url, timeout=cls.OPENTIMEOUT)
        asyncio.Task(closefile())
        return file


class BlockCachedFile:
    """This type of file handle reads blocks via the global block cache."""
    def __init__(self, url, blocksize):
        self.url = url 
        self.blocksize = blocksize
        

    async def preload(self):
        # since len() can't be async, we read the length here.
        self.size = await IOService.read_len(self.url)
        

    def __len__(self):
        return self.size
    

    async def __getblocks(self, idxslice):
        # Process the __getitem__ parameters.
        start, end, stride = idxslice.indices(len(self))
        assert stride == 1 and start >= 0 and end >= 0
        
        firstblock, lastblock = start//self.blocksize, end//self.blocksize
        
        # For the blocks we actually need (rarely more than one), we start parallel requests.
        blockids = list(range(firstblock, lastblock+1))
        IOService.do_readahead = True
        tasks = [IOService.read_block(self.url, blockid) for blockid in blockids]
        blocks = await asyncio.gather(*tasks)
        
        # finally, we assemble the result. There are some unnecessary copies here.
        parts = []
        for blockid, block in zip(blockids, blocks): 
            offset = blockid*self.blocksize
            parts.append(block[max(0, start-offset) : max(0, end-offset)])
        return b''.join(parts)


    async def __getitem__(self, idx):
        if isinstance(idx, slice):
            return await self.__getblocks(idx)
        else:
            return (await self[idx:idx+1])[0]


class FullFile:
    """This type of file handle loads and keeps a full copy of the file content, bypassing the cache."""

    def __init__(self, url, timeout):
        self.url = url
        self.timeout = timeout


    async def preload(self):
        # in this mode, we just preload the full file in the beginning, into a per-file cache.
        f = await nanoroot.XRDFile().load(url, timeout=self.timeout)
        self.buf = await f[:]


    def __len__(self):
        return len(self.buf)


    async def __getitem__(self, idx):
        return self.buf[idx]

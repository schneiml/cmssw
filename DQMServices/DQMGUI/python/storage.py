import re
import time
import zlib
import struct
import socket
import asyncio
import tempfile
import aiosqlite
import subprocess

from compressing import GUIBlobCompressor
from helpers import get_absolute_path


class GUIDataStore:

    __DBSCHEMA = """
    BEGIN;
    CREATE TABLE IF NOT EXISTS samples(dataset, run, lumi, filename, fileformat, menamesid, meinfosid);
    CREATE INDEX IF NOT EXISTS samplelookup ON samples(dataset, run, lumi);
    CREATE UNIQUE INDEX IF NOT EXISTS uniquesamples on samples(dataset, run, lumi, filename, fileformat);
    CREATE TABLE IF NOT EXISTS menames(menamesid INTEGER PRIMARY KEY, menameblob);
    CREATE UNIQUE INDEX IF NOT EXISTS menamesdedup ON menames(menameblob);
    CREATE TABLE IF NOT EXISTS meinfos(meinfosid INTEGER PRIMARY KEY, meinfosblob);
    COMMIT;
    """

    __db = None
    __lock = asyncio.Lock()

    compressor = GUIBlobCompressor()


    @classmethod
    async def initialize(cls, connection_string='../data/directory.sqlite', in_memory=False):
        """
        Creates DB from schema if it doesn't exists and open a connection to it.
        If in_memory is set to Trye, an in memory DB will be used.
        """

        if in_memory:
            connection_string = ':memory:'
        else:
            connection_string = get_absolute_path(connection_string)

        cls.__db = await aiosqlite.connect(connection_string)
        await cls.__db.executescript(cls.__DBSCHEMA)


    @classmethod
    async def destroy(cls):
        try:
            await cls.__db.close()
        except:
            pass


    @classmethod
    async def get_samples(cls, run, dataset):
        if run:
            run = '%%%s%%' % run
        if dataset:
            dataset = '%%%s%%' % dataset

        cursor = None

        if run == dataset == None:
            return []
        elif run != None and dataset != None:
            sql = 'SELECT DISTINCT run, dataset FROM samples WHERE dataset LIKE ? AND run LIKE ?'
            cursor = await cls.__db.execute(sql, (dataset, run))
        elif run != None:
            sql = 'SELECT DISTINCT run, dataset FROM samples WHERE run LIKE ?'
            cursor = await cls.__db.execute(sql, (run,))
        elif dataset != None:
            sql = 'SELECT DISTINCT run, dataset FROM samples WHERE dataset LIKE ?'
            cursor = await cls.__db.execute(sql, (dataset,))

        rows = await cursor.fetchall()
        await cursor.close()

        return rows


    @classmethod
    async def get_me_names_list(cls, dataset, run):
        """
        me_list format for an ME (ME/Path/mename) that has 2 QTests and an efficiency flag set looks like this:
        ME/Path/mename
        ME/Path/mename\0.qtest1
        ME/Path/mename\0.qtest2
        ME/Path/mename\0e=1
        This function returns a list of contents in provided run/dataset combination, in a format explained above.
        """
        # For now adding a LIMIT 1 because there might be multiple version of the same file.
        sql = 'SELECT menameblob FROM samples JOIN menames ON samples.menamesid = menames.menamesid WHERE run = ? AND dataset = ? LIMIT 1;'

        cursor = await cls.__db.execute(sql, (int(run), dataset))
        row = await cursor.fetchone()
        await cursor.close()

        if not row:
            # Blob doesn't exist, we should probably try to import
            return None

        names_list = await cls.compressor.uncompress_names_blob(row[0])
        return names_list


    @classmethod
    async def get_filename_fileformat_names_infos(cls, dataset, run, lumi=0):
        # For now adding a LIMIT 1 because there might be multiple version of the same file.
        sql = '''
        SELECT
            filename,
            fileformat,
            menameblob,
            meinfosblob
        FROM
            samples
            JOIN menames ON samples.menamesid = menames.menamesid
            JOIN meinfos ON samples.meinfosid = meinfos.meinfosid
        WHERE
            run = ?
        AND 
            lumi = ?
        AND 
            dataset = ?
        LIMIT 1;
        '''

        cursor = await cls.__db.execute(sql, (int(run), int(lumi), dataset))
        row = await cursor.fetchone()
        await cursor.close()

        if not row:
            # Blobs don't exist, we should probably try to import
            return None

        filename = row[0]
        fileformat = row[1]
        names_list = await cls.compressor.uncompress_names_blob(row[2])
        infos_list = await cls.compressor.uncompress_infos_blob(row[3])

        return (filename, fileformat, names_list, infos_list)


    @classmethod
    async def get_samples_count(cls):
        sql = 'SELECT COUNT(*) FROM samples;'
        cursor = await cls.__db.execute(sql)
        row = await cursor.fetchone()
        await cursor.close()
        return row[0]


    @classmethod
    async def get_sample_file_info(cls, dataset, run, lumi=0):
        """
        Returns a full path and a format of a ROOT file containing plots of a given sample. 
        Returns None if given sample doesn't exist.
        """

        sql = 'SELECT filename, fileformat FROM samples WHERE dataset = ? AND run = ? AND lumi = ?;'
        cursor = await cls.__db.execute(sql, (dataset, int(run), int(lumi)))
        row = await cursor.fetchone()

        await cursor.close()

        if row:
            return row[0], row[1]
        else:
            return None


    @classmethod
    async def register_samples(cls, samples):
        """
        Adds multiple samples to the database.
        Sample is declared here: data_types.SampleFull
        """

        sql = 'INSERT OR REPLACE INTO samples VALUES(?,?,?,?,?,NULL,NULL);'
        await cls.__db.executemany(sql, samples)
        await cls.__db.commit()


    @classmethod
    async def add_blobs(cls, names_blob, infos_blob, dataset, filename, run, lumi=0):
        """Adds me list blob and infos blob to a DB in a single transaction."""

        try:
            await cls.__lock.acquire()
            await cls.__db.execute('BEGIN;')

            await cls.__db.execute('INSERT OR IGNORE INTO menames (menameblob) VALUES (?);', (names_blob,))
            cursor = await cls.__db.execute('SELECT menamesid FROM menames WHERE menameblob = ?;', (names_blob,))
            row = await cursor.fetchone()
            names_blob_id = row[0]

            await cls.__db.execute('INSERT OR IGNORE INTO meinfos (meinfosblob) VALUES (?);', (infos_blob,))
            cursor = await cls.__db.execute('SELECT meinfosid FROM meinfos WHERE meinfosblob = ?;', (infos_blob,))
            row = await cursor.fetchone()
            infos_blob_id = row[0]

            sql = 'UPDATE samples SET menamesid = ?, meinfosid = ? WHERE run = ? AND lumi = ? AND dataset = ? AND filename = ?;'
            await cls.__db.execute(sql, (names_blob_id, infos_blob_id, int(run), int(lumi), dataset, filename))

            await cls.__db.execute('COMMIT;')
        except Exception as e:
            print(e)
            try:
                await cls.__db.execute('ROLLBACK;')
            except:
                pass
        finally:
            cls.__lock.release()

"""
This service provides the logic of the API endpoints. It relies on other services to perform certain tasks.
"""

import time
import json
import struct
from collections import defaultdict

from async_lru import alru_cache

from rendering import GUIRenderer
from storage import GUIDataStore
from importing import GUIImporter
from helpers import PathUtil, get_api_error, binary_search
from data_types import Sample, RootDir, RootObj, RootDirContent, RenderingInfo

from layouts.layout_manager import LayoutManager


class GUIService:

    store = GUIDataStore()
    renderer = GUIRenderer()
    importer = GUIImporter()
    layouts_manager = LayoutManager()

    @classmethod
    @alru_cache(maxsize=10)
    async def get_samples(cls, run, dataset):
        if run == '':
            run = None
        if dataset == '':
            dataset = None
        
        results = await cls.store.get_samples(run, dataset)
        return [Sample(*x) for x in results]


    @classmethod
    @alru_cache(maxsize=10)
    async def get_archive(cls, run, dataset, path, search):
        """
        Returns a directory listing for run/dataset/path combination.
        Search is performed on ME name if it is provided.
        Layout property is the name of the layout the ME is coming from. If layout property is null, 
        it means that ME is not coming from a layout. 
        """
        # Path must end with a slash
        if path and not path.endswith('/'):
            path = path + '/'
        
        # Get a list of all MEs
        lines = await cls.__get_melist(run, dataset)

        regex = re.compile(search) if search else None

        # dir is a dict where key is subdir name and value is a count of 
        # how many MEs are inside that subdir (in all deeper levels)
        dirs = defaultdict(int)
        objs = set()

        path_util = PathUtil()

        # TODO: This is now a linear search over a sorted list: optimize this!!!
        for x in range(len(lines)):
            line = lines[x].decode("utf-8")
            path_util.set_path(line)
            subsequent_segment = path_util.subsequent_segment_of(path)
            if subsequent_segment:
                if regex and not regex.match(line.split('/')[-1]):
                    continue # Regex is provided and ME name doesn't match it

                if '\0' in line:
                    continue # This is a secondary item, not a main ME name

                if subsequent_segment.is_file:
                    objs.add(RootObj(name=subsequent_segment.name, path=path + subsequent_segment.name, layout=None))
                else:
                    dirs[subsequent_segment.name] += 1

        # Add MEs from layouts
        # Layouts will be filtered against the search regex on their destination name.
        # Non existant sources will still be attempted to be displayed resulting in 
        # 'ME not found' string to be rendered.
        for layout in cls.layouts_manager.get_layouts():
            path_util.set_path(layout.destination)
            subsequent_segment = path_util.subsequent_segment_of(path)
            if subsequent_segment:
                if regex and not regex.match(layout.destination.split('/')[-1]):
                    continue # Regex is provided and destination ME name doesn't match it

                if subsequent_segment.is_file:
                    objs.add(RootObj(name=subsequent_segment.name, path=layout.source, layout=layout.name))
                else:
                    dirs[subsequent_segment.name] += 1

        # Transform dirs into a list of RootDir objects
        dirs = [RootDir(name=key, me_count=dirs[key]) for key in dirs]

        # Format results to a named tuple
        data = RootDirContent(dirs, objs)
        return data


    @classmethod
    async def get_rendered_image(cls, me_descriptions, options):
        """options are defined here: data_types.RenderingOptions"""

        options.efficiency = False
        rendering_infos = []

        for me in me_descriptions:
            filename, me_list, me_infos = await cls.__get_filename_melist_offsets(me.run, me.dataset)

            # Find the index of run/dataset/me in me list blob. 
            # The index in me list will correspond to the index in offsets list.
            me_info = None
            index = binary_search(array=me_list, target=bytes(me.path, 'utf-8'))
            if index != -1:
                me_info = me_infos[index]
            else:
                continue
            
            # If efficiency flag is set for at least one of the MEs, it will be set for an overlay
            if not options.efficiency:
                efficiency_line = bytes('%s\0e=1' % me.path, 'utf-8')
                options.efficiency = binary_search(array=me_list, target=efficiency_line) != -1

            rendering_infos.append(RenderingInfo(filename=filename, path=me.path, me_info=me_info))

        if not rendering_infos: # No MEs were found
            if options.json:
                return None
            else:
                return await cls.renderer.render_string('ME not found', width=options.width, height=options.height)

        return await cls.renderer.render(rendering_infos, options)


    @classmethod
    async def get_rendered_json(cls, me_descriptions, options):
        """
        Uses out of process renderer to get JSON representation of a ROOT object.
        Adds additional property to the resulting JSON called dqmProperties
        """

        data = await cls.get_rendered_image(me_descriptions, options)
        if data:
            data = data.decode('utf-8')
            obj = json.loads(data)
            obj['dqmProperties'] = { 'efficiency': options.efficiency }
            return obj
        else:
            return get_api_error(message='ME not found')


    @classmethod
    @alru_cache(maxsize=10)
    async def __get_melist(cls, run, dataset):
        lines = await cls.store.get_me_list_blob(run, dataset)

        if lines == None:
            # Import and retry
            success = await cls.importer.import_blobs(run, dataset)
            if success: 
                # Retry
                lines = await cls.store.get_me_list_blob(run, dataset)

        return lines if lines else []


    @classmethod
    @alru_cache(maxsize=10)
    async def __get_filename_melist_offsets(cls, run, dataset):
        blobs_and_filename = await cls.store.get_blobs_and_filename(run, dataset)

        if blobs_and_filename == None:
            # Import and retry
            success = await cls.importer.import_blobs(run, dataset)
            if success:
                # Retry
                blobs_and_filename = await cls.store.get_blobs_and_filename(run, dataset)

        if blobs_and_filename:
            filename, me_list, me_infos = blobs_and_filename
            return (filename, me_list, me_infos)
        else:
            return None
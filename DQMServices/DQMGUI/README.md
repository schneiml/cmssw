# DQMServices/DQMGUI

_Note: This is not the production DQMGUI._

This package contains experimental code for a DQMGUI similar to https://cmsweb.cern.ch/dqm/offline/ , but as part of CMSSW.

There are multiple relevant parts:
- The _render service_ in `bin/render.cc`, extracted from the classic DQMGUI: https://github.com/rovere/dqmgui
- The _render plugins_ in `plugins/`, traditionally hosted on https://github.com/dmwm/deployment/tree/master/dqmgui/style
- A storage backend.
- A web server.
- A HTML frontend.

## The render service

The histogram rendering using ROOT and the render plugins is done in a separate process in the classic DQMGUI. This package contians a simplified version of this process. `render.cc` compiles into a standalone program, `dqmRender`, that listens on a UNIX socket. A client (e.g. the GUI webserver) can request rendering a histogram there, by sending a request consisting of some metadata (some of which is a relict of the past and not actually used) and an arbitrary number of ROOT objects serialized into a `TBufferFile` buffer: The fisrt is the main object, the remaining ones are reference histograms that will be overlayed in different colors. (Some code for rendering _built-in references_ stored with the main object might remain, but this mode is no longer supported -- in line with CMSSW no longer supporting refrence hisotgrams in the `MonitorElement`.) The response is a PNG-compressed bitmap. All messages use a simple framing format of first sending the length, then the actual data, sometimes nested.

A client that implements this protocol is implemented in `python/render.py`.

The render process is single-threaded and does not do any IO apart from the UNIX socket. Many of them can be launched in parallel. They might crash now and then (because ROOT), so some  precautions should be taken to restart them if they fail.

Since the `TBufferFile` data does not contain streamers, but we also don't want to repack all data into the latest format before rendering, the renderer has a mechanism to load streamers. This is done by passing a file name with the request, the renderer will simply open this file once (which reads the streamers as a side effect). This should only be done as needed, since it is quite slow (compared to the actual rendering).

### The render plugins

Render plugins are C++ classes that can modify how histograms are displayed. They are located in the `plugins/` folder, even though they are not EDM plugins. The render plugins are loaded dynamically by the render service (typically on startup, by passing the name of a `.so` with renderplugins to load). We have quite a lot of them, and they are the main reason to keep using this renderer (compared to e.g. switching to JSROOT).

### Compiling this code

The `scram` `BuildFile`s should do everything. But it is also not too hard to do it without scram, given a working installation of ROOT. There are no dependencies on other parts of CMSSW.

`render.cc` needs ROOT includes and a lot of ROOT libraries for linking, as well as `libpng`, which is a bit picky about versions (`-lpng15` was required here to work). It also needs to be linked dynamically to the render plugin base class so that the plugin registration works. This is done using a rule to link to this package, `DQMServices/DQMGUI`, in `BuildFile.xml`.

The render plugins need to register with their base class so that they can be called. This is done using a global variable in `DQMRenderPlugin.cc`. It is located in `src/` to get it compiled into a shared library, that can then share this state between the renderer and the render plugins.

The render plugins are compiled separately in `plugins/` and linked dynamically against `DQMRenderPlugin.cc` (via `DQMServices/DQMGUI` in the `BuildFile`). This results in a new shared library `.so`, which can then be dynamically loaded at runtime in `render.cc` (via `dlopen`), and all the plugins will automatically register. The render plugins are also linked against some other stuff from ROOT that they might need. They could actually depend on other CMSSW code now where it makes sense (e.g. detector geometries).

There is some hacky code in `render.py` that locates the `.so` with the render plugins and passes it to `render.cc` as a command line argument.

## The storage backend

The storage backend is based on legacy, `TDirectory` ROOT files. The code is in `rootstorage.py`. It keeps a SQLite database of metadata, about _samples_ (run/dataset/lumi, effectively _files_, for the legacy format), and _ME lists_, which represent the MEs present in a sample. These are stored compressed to make their size manageable. The ME list is built on first access; this makes it feasible to register all ~80000 files that we have on EOS at the moment as samples.

The storage backend is based on `uproot`, it never uses actual ROOT. To produce the `TBufferFile` format for the renderer, there is some custom byte-level packing code to add the required headers.

## The web server

There is a simple web server in `server.py`. It simply maps the classic DQMGUI API to the matching calls in the storage layer. It also does rendering using the render service.  The request parsing is very bad, so it fails in some cases and probably has lots of security issues.

The server is _threaded_, which allows it to do some of the IO waiting in parallel. But Python threading is not very efficient, so it limits at around 100 requests/s, which is less than the renderers could handle.

The server is started like this:
```
python3 DQMServices/DQMGUI/python/server.py
```

It will listen on `http://localhost:8889` (and you can't just change that, see below), and It will automatically create a DB file in `DQMServices/DQMGUI/data/` and populate it using data from EOS. 

## The HTML frontend

The frontend is developed here: https://github.com/cms-DQM/dqmgui_frontend

This package contains compiled code from there, which is served from the web server to get a working GUI. It is hardcoded to `localhost:8889`, so you can't easily change the port number in the server.

## API documentation

This is the future version of the DQM GUI API and it is preferred for all services over the legacy API.

**This API specification is not yet final and is subject to change!!!**

#### Samples endpoint

Returns run/dataset pairs available in the GUI. Both arguments are optional and support regex.

`http://localhost:8889/api/v1/samples?run=295120&dataset=Run2017A`

```json
{
  "data": [
    {
      "run": 295120,
      "dataset": "/Cosmics/Run2017A-PromptReco-v1/DQMIO"
    },
    {
      "run": 295120,
      "dataset": "/StreamExpressCosmics/Run2017A-Express-v1/DQMIO"
    }
  ]
}
```

#### ROOT file directory listing endpoint

Run, full dataset and a path has to be provided in the URL.

If `layout` is `null`, ME is not coming from a layout. Otherwise, `layout` contains the name of the layout this ME comes from. 

`http://localhost:8889/api/v1/archive/316142/StreamExpress/Run2018A-Express-v1/DQMIO/PixelPhase1`

```json
{
  "data": [
    {
      "subdir": "Summary"
    },
    {
      "subdir": "ClusterShape"
    },
    {
      "name": "num_feddigistrend_per_LumiBlock_per_FED",
      "path": "PixelPhase1/num_feddigistrend_per_LumiBlock_per_FED",
      "layout": null
    },
    {
      "name": "deadRocTotal",
      "path": "PixelPhase1/deadRocTotal",
      "layout": null
    },
  ]
}
```

#### Layouts endpoint

Returns all layouts with the same name. Used for quick collections.

`http://localhost:8889/api/v1/layouts?name=layout1`

```json
{
  "data": [
    {
      "source": "Hcal/TPTask/EtEmul/TTSubdet/HBHE",
      "destination": "Hcal/Layouts/EtEmul/TP/TTSubdet/HBHE_changed_name"
    }
  ]
}
```

#### Rendering endpoint

Renders a PNG of a histogram.

`http://localhost:8889/api/v1/render/316142/StreamExpress/Run2018A-Express-v1/DQMIO/PixelPhase1/EventInfo/reportSummaryMap?w=266&h=200&stats=false&norm=false&errors=true`

#### Overlay rendering endpoint

Overlays multiple (or one) histograms and renders an overlay to a PNG.

`http://localhost:8889/api/v1/render_overlay?obj=archive/316142/StreamExpress/Run2018A-Express-v1/DQMIO/PixelPhase1/EventInfo/reportSummary&obj=archive/316144/StreamExpress/Run2018A-Express-v1/DQMIO/PixelPhase1/EventInfo/reportSummary&w=266&h=200&stats=false&norm=false&errors=true`

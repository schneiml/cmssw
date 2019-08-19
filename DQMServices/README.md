DQM Services
============

These packages contain functionality for DQM that is not specific to any data, subsystem or detector. This is the "DQM Framework", provided by the DQM group to allow detector specific code (mostly in `DQM`, `DQMOffline/` and `Validation/` to interact with the DQM infrastructure (DQM GUI, Tier0 processing, Online DQM, etc.).

This document contains forward references. Terms may be used before they are defined.


Package Contents
----------------

- `ClientConfig/`: Random shared code for harvesting/client code. This includes the `DQMGenericClient` shared harvesting module and some `QualityTest` functionality.
- `Components/`: Collection of (independent) DQM plugins that handle core functionality. This includes
    - `QualityTester`: TODO: What does it do? Multiple instances running in standard HARVESTING.
    - `DQMDcsInfoClient`: Populates one of the DCS info plots (TODO: Which?)
    - `DQMDcsInfo`: Populates another DCS info plot (TODO: Which?)
    - `DQMDaqInfo`: TODO: something
    - `DQMEventInfo`: TODO: something
    - `DQMFEDIntegrityClient`: TODO: something
    - `DQMFileReader`: Reads a legacy format ROOT file as a reference for a few `Validation/RecoEgamma` workflows.
    - `DQMMessageLogger`: Creates histograms of number of EDM error messages.
    - `DQMMessageLoggerClient`: Some sort of post-processing on these hisetograms.
    - `DQMFileSaver`: Triggers legacy format saving of DQM output in harvesting jobs (and maybe online?)
    - `DQMLumiMonitor`: Some sort of SiPixel based luminosity measurement. Does not belong into this package. Used online only.
    - `DQMScalInfo`: Reports some data from Lumi/L1T Scalers. Used online.
    - `DQMProvInfo`: Populates the `Info/*Info` histograms.
    - `DQMStoreStats`: Provides some DQM self-monitoring (memory etc.) Not used in production.
    - `MEtoMEComparitor`: TODO: does something. Not used in production sequences.
    - `EDMtoMEConverter`: Reads histograms from EDM event files and puts them into DQM. Used for AlCa.
    - `python/`: TODO: some sort of web interface for something?
    - `scripts/`: Tools to acces DQMGUI and to compare/inspect DQM data files.
    - `test/`: DQM unit tests.
- `Core/`: Header files used by other DQM modules, implementation of `DQMStore` and `MonitorElement`. No plugins.
- `Diagnostic/`: TODO: some ancient HDQM?
- `FwkIO/`: The DQMIO input and output modules.
- `FileIO/`: More modules triggering saving to legacy files, probably for online. See also: `Components/DQMFileSaver`.
- `StreamerIO/`: Raw data input for online, plus general online DQM tools. Includes the bridge to `DQM^2` (`esMonitoring.py`)



Historical: DQM before the 2019 migration
-----------------------------------------

_a.k.a. 2014/2015 threaded DQM_

_This is information about how DQM used to work. Even though it no longer works that way, and some of the things mentioned don't exist any more, lots of terminology and comments refer to this old design._

### Which components exist?

#### Plugins

DQM code runs as CMSSW plugins there are two main types of plugins: *Analyzers* and *Harvesters*. Both exist in a number of technical variations, defined by the base class used.

- Analyzers:
    - `DQMEDAnalyzer`: Based on `edm::stream::EDAnalyzer` (2015) or `edm::one::EDProducer` (2018), the recommended default base class.
    - `one::DQMEDAnalyzer`: Based on `edm::one`, to be used if `edm::one` behaviour is specifically required.
        - `DQMRunEDProducer` is used if the template parameters indicate that *no* lumi-based histograms are needed.
        - `DQMLumisEDProducer` is used if the template parameters indicate that lumi-based histograms are needed.
    - `DQMGlobalEDAnalyzer`: largely independent `edm::global::EDAnalyzer`. Only recommended for specific applications (DQM@HLT).
    - `edm::EDAnalyzer`: EDM legacy base class, still widely used.
- Harvesters:
    - `DQMEDHarvester`: Recommended base class for harvesters, `edm::one::EDProducer` based.
    - `edm::EDAnalyzer`: EDM legacy base class, still widely used.

The `DQMStore` lives in CMSSW as a `edm::Service` singleton instance. For DQMIO, there is an `OutputModule` (`DQMRootOutputModule`), and an `InputSource` (`DQMRootSource`). 

#### Library classes

Various functionalities are provided in classes which can be used by the CMSSW plugins. The main ones are

- `DQMStore`, to interface with the `DQMStore` service and register (book) `MonitorElement`s with the framwork.
- `MonitorElement`, an object representing a single histogram (or scalar value) of a monitored quantity. In practise, this is a thin wrapper around a ROOT `TH1` instance.
- `QCriterion`, an automated check/comparison on a histogram. Many subclasses for specific checks exist.

#### File formats

DQM data (mostly histograms, more specifically `MonitorElement`s) can be save in multiple different formats. The formats differ in which features of MEs they can express/persist, and how slow IO is.

- _DQMIO_: this is the "official" DQM data format. It is used for the DQMIO datatier stored after the RECO step in processing jobs. Histograms are stored in ROOT `TTree`s, which makes IO reasonably fast. EDM metadata is preserved, ME metadata is properly encoded and multiple runs/lumisections can be store in a single file. Obscure ME options (reference histograms, flags, etc.) are not preserved.
- _Legacy ROOT_ (`DQM_*.root`): this is the most "popular" DQM data format. It is used for the uploads to DQMGUI and typically saved after the HARVESTING step. Histograms are stored in a `TDirectory`, which makes IO very slow but easy. All ME metadata is preserved (as pseudo-XML `TString`s in the `TDirectory`), but run/lumi metadata is not supported everywhere.
- _Legacy ProtoBuf (PB)_: this format is similar to legacy ROOT, but instead of the slow `TDirectory`, it uses a Google ProtoBuf based container. ROOT objects are serialized into `TBufferFile` objects, which allow very fast IO but no schema evolution (not suitable for archiving). Used for the `fastHadd` process in DQM@HLT and historically for batch impor/export in DQMGUI.
- _MEtoEDM/edm Event format_: Using dedicated plugins, DQM histograms can be transformed into EDM products and saved into, or read from, the EDM event files. Support for obscure ME features is unknown, IO performance is also unknown, full EDM metadata is preserved. This format is used in AlCa workflows between ALCARECO and ALCAHARVEST steps (TODO: confirm, very little is know about this usage).
- _DQMGUI index format_: Not part of CMSSW. 
- (_DQMNET_): Not a file format. Network protocol used between CMSSW and DQMGUI in online live mode. Based on `TBufferFile`.

Data is archived for eternity by CMS computing in DQMIO format (TODO: confirm the precise rules), in legacy ROOT format by the DQM group (CASTOR tape backups, usually not available for reading), and in the DQMGUI index (hosted by CMSWEB, on disk, instant access to any ME).

#### Processing environments

DQM code runs in various places during CMS data processing. How much code runs, and how much processing time is spent doing DQM, varies by orders of magnitude. In rough order of amount of code and time spent, there are

- _RECO_: Processing step in all CMS offline processing (Prompt, Express, ReReco, Monte Carlo, RelVal) where DQM data is extracted from events ("map" in the Map-Reduce pattern). Data is split into an arbitrary number of jobs and processed massively parallel.
- _HARVESTING_: Processing step in all CMS offline processing where data saved by the RECO step in DQMIO format is read and reduced into the final output ("reduce" in the Map-Reduce pattern). Full run data is available. Produces a legacy ROOT output file and uploads it to DQMGUI.
- _ALCARECO_: Like RECO, but on specical data and with special code for AlCa. Saves data in MEtoEDM format (among others?).
- _ALCAHARVEST_: Like HARVESTING, but for ALCARECO data.
- _online DQM_: Runs constantly on a small fraction of data directly from HLT while CMS is taking data, on dedicated hardware at Point 5 (4 physical machines). Analyzes a small number of events and live-streams histograms to the online DQMGUI. An identical _playback_ system, also running 24/7 on a small set of stored data, exists as well. 
- _DQM@HLT_: A small amount of DQM code runs on all HLT nodes to collect statistics over all events at the input of HLT. These histograms get saved into files in the legacy ProtoBuf format, are merged/reduced using the `fastHadd` utility on DAQ systems, and streamed into the online DQM system, where they are repacked and continue like normal online DQM. The output is used by a few HLT experts.
- _DQMIO merge_: Between RECO and HARVESTING, DQMIO files are partially merged into bigger files. No plugins run in this step, only DQMIO input and output.





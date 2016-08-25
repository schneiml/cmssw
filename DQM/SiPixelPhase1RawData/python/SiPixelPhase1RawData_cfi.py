import FWCore.ParameterSet.Config as cms
from DQM.SiPixelPhase1Common.HistogramManager_cfi import *

# the Framework can figure out the FED range automatically, but here we want to
# do things by hand, so we need to know the range now and then.
# this is a bit problematic, since it needs to be adapted for Phase0/1.
SiPixelPhase1RawData_MinFED = 0
SiPixelPhase1RawData_MaxFED = 40

SiPixelPhase1RawDataFEDPresent = DefaultHisto.clone(
  name = "fedpresent",
  title = "FED is present",
  xlabel = "FED#",
  dimensions = 0,
  specs = cms.VPSet(
    Specification().groupBy("FED")
                   .groupBy("", "EXTEND_X")
                   .save(),
  )
)

SiPixelPhase1RawDataNErrors = DefaultHisto.clone(
  name = "nerrors",
  title = "Number of errors",
  xlabel = "#errors",
  range_min = 0, range_max = 30, range_nbins = 30,
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("FED").save(),
  )
)

SiPixelPhase1RawDataFIFOFull = DefaultHisto.clone(
  name = "fifofull",
  title = "Type of FIFO full",
  xlabel = "FIFO type",
  range_min = 0.5, range_max = 7.5, range_nbins = 7,
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("FED").save(),
  )
)

SiPixelPhase1RawDataTBMMessage = DefaultHisto.clone(
  name = "tbmmessage",
  title = "TBM trailer message",
  xlabel = "TBM message",
  range_min = 0.5, range_max = 7.5, range_nbins = 7,
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("FED").save(),
  )
)

SiPixelPhase1RawDataTBMType = DefaultHisto.clone(
  name = "tbmtype",
  title = "Type of TBM trailer",
  xlabel = "TBM type",
  range_min = -0.5, range_max = 4.5, range_nbins = 4,
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("FED").save(),
  )
)

SiPixelPhase1RawDataEventNumber = DefaultHisto.clone(
  name = "eventnumber",
  title = "Last Event Number",
  xlabel = "",
  ylabel = "Event#",
  range_min = 0, range_max = 1, range_nbins = 1,
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("FED").save(),
  )
)

SiPixelPhase1RawDataEventSize = DefaultHisto.clone(
  name = "eventsize",
  title = "Last Event Size",
  xlabel = "",
  ylabel = "Event Size",
  range_min = 0, range_max = 1, range_nbins = 1,
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("FED").save(),
  )
)

SiPixelPhase1RawDataChannelNErrors = DefaultHisto.clone(
  name = "nerrors_per_channel",
  title = "Number of errors per FED Channel",
  xlabel = "FED Channel",
  range_min = 0, range_max = 40, range_nbins = 40,#TODO: real max channel here
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("FED").save()
                   .groupBy("", "EXTEND_Y").save(),
  )
)

# TODO: this cannot work in offline. It also can't really work in the old DQM
SiPixelPhase1RawDataChannelLastError = DefaultHisto.clone(
  name = "lasterror_per_channel",
  title = "Last Error per FED Channel",
  xlabel = "FED Channel",
  range_min = 0, range_max = 40, range_nbins = 40,#TODO: real max channel here
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("FED").save()
                   .groupBy("", "EXTEND_Y").save(),
  )
)

SiPixelPhase1RawDataTypeNErrors = DefaultHisto.clone(
  name = "nerrors_per_type",
  title = "Number of Errors per Type",
  xlabel = "Error Type",
  range_min = 0, range_max = 20, range_nbins = 20,#TODO: weird logic to map error types here.
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("FED").save()
                   .groupBy("", "EXTEND_Y").save(),
  )
)

SiPixelPhase1RawDataConf = cms.VPSet(
  SiPixelPhase1RawDataFEDPresent,
  SiPixelPhase1RawDataNErrors,
  SiPixelPhase1RawDataFIFOFull,
  SiPixelPhase1RawDataTBMMessage,
  SiPixelPhase1RawDataTBMType,
  SiPixelPhase1RawDataEventNumber,
  SiPixelPhase1RawDataEventSize,
  SiPixelPhase1RawDataChannelNErrors,
  SiPixelPhase1RawDataChannelLastError,
  SiPixelPhase1RawDataTypeNErrors,
)

SiPixelPhase1RawDataAnalyzer = cms.EDAnalyzer("SiPixelPhase1RawData",
        src = cms.InputTag("siPixelDigis"),
        histograms = SiPixelPhase1RawDataConf,
        geometry = SiPixelPhase1Geometry
)

SiPixelPhase1RawDataHarvester = cms.EDAnalyzer("SiPixelPhase1Harvester",
        histograms = SiPixelPhase1RawDataConf,
        geometry = SiPixelPhase1Geometry
)

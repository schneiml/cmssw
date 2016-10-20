import FWCore.ParameterSet.Config as cms
from DQM.SiPixelPhase1Common.HistogramManager_cfi import *

SiPixelPhase1OnlineTimingProcessingTime = DefaultHisto.clone(
  name = "processingtime",
  title = "Online Processing time (Event)",
  range_min = 0, range_max = 1, range_nbins = 100,
  xlabel = "s/Event",
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("Lumisection")
                   .reduce("MEAN") 
                   .groupBy("", "EXTEND_X")
                   .save(),
    Specification().groupBy("").save()
  )
)

SiPixelPhase1OnlineTimingProcessingTimeLs = DefaultHisto.clone(
  name = "processingtime_ls",
  title = "Online Processing time (Lumisection)",
  range_min = 0, range_max = 1, range_nbins = 100,
  xlabel = "s/Lumisection",
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("Lumisection")
                   .reduce("MEAN") 
                   .groupBy("", "EXTEND_X")
                   .save(),
  )
)

SiPixelPhase1OnlineTimingEventRate = DefaultHisto.clone(
  name = "realeventrate",
  title = "Eventrate estimated from time between events",
  range_min = -1, range_max = 1, range_nbins = 1000,
  xlabel = "s/Event",
  dimensions = 1,
  specs = cms.VPSet(
    Specification().groupBy("Lumisection")
                   .reduce("MEAN") 
                   .groupBy("", "EXTEND_X")
                   .save(),
    Specification().groupBy("").save()
  )
)

SiPixelPhase1OnlineTimingConf = cms.VPSet(
  SiPixelPhase1OnlineTimingProcessingTime,
  SiPixelPhase1OnlineTimingProcessingTimeLs,
  SiPixelPhase1OnlineTimingEventRate
)

SiPixelPhase1OnlineTimingAnalyzer = cms.EDAnalyzer("SiPixelPhase1OnlineTiming",
        histograms = SiPixelPhase1OnlineTimingConf,
        geometry = SiPixelPhase1Geometry
)
SiPixelPhase1OnlineTimingHarvester = cms.EDAnalyzer("SiPixelPhase1Harvester",
        histograms = SiPixelPhase1OnlineTimingConf,
        geometry = SiPixelPhase1Geometry
)

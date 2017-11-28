import FWCore.ParameterSet.Config as cms

SiStripMonitorFilter = DQMStep1Analyzer("SiStripMonitorFilter",
    FilterProducer = cms.string('ClusterMTCCFilter')
)

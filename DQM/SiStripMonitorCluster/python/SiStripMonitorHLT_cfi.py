import FWCore.ParameterSet.Config as cms

SiStripMonitorHLT = DQMStep1Analyzer("SiStripMonitorHLT",
    HLTProducer = cms.string('trigger')
)

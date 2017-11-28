import FWCore.ParameterSet.Config as cms

ctppsPixelDQMSource = DQMStep1Analyzer("CTPPSPixelDQMSource",
    tagRPixDigi = cms.InputTag("ctppsPixelDigis", ""),
    tagRPixCluster = cms.InputTag("ctppsPixelClusters", ""),  
    RPStatusWord = cms.untracked.uint32(0x8000), # rpots included in readout
    verbosity = cms.untracked.uint32(0)
)

import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
l1tStage2uGt = DQMEDAnalyzer('L1TStage2uGT',
    l1tStage2uGtSource = cms.InputTag("gtStage2Digis"),    
    monitorDir = cms.untracked.string("L1T/L1TStage2uGT"),
    verbose = cms.untracked.bool(False)
)

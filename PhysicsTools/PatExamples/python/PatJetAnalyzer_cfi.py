import FWCore.ParameterSet.Config as cms

analyzePatJets = DQMStep1Analyzer("PatJetAnalyzer",
  src  = cms.InputTag("cleanPatJets"),                              
  corrLevel = cms.string("L3Absolute")
)                               

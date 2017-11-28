import FWCore.ParameterSet.Config as cms

#
# module to make simple analyses of jets
#
analyzeJet = DQMStep1Analyzer("TopJetAnalyzer",
    input   = cms.InputTag("selectedPatJets"),
    verbose = cms.bool(True)
)



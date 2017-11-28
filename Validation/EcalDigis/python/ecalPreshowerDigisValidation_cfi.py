import FWCore.ParameterSet.Config as cms

ecalPreshowerDigisValidation = DQMStep1Analyzer("EcalPreshowerDigisValidation",
    ESdigiCollection = cms.InputTag("simEcalPreshowerDigis"),
    verbose = cms.untracked.bool(False)
)



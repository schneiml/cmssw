import FWCore.ParameterSet.Config as cms

ecalBarrelDigisValidation = DQMStep1Analyzer("EcalBarrelDigisValidation",
    EBdigiCollection = cms.InputTag("simEcalDigis","ebDigis"),
    verbose = cms.untracked.bool(False)
)



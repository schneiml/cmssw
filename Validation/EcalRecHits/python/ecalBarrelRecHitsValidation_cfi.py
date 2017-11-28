import FWCore.ParameterSet.Config as cms

ecalBarrelRecHitsValidation = DQMStep1Analyzer("EcalBarrelRecHitsValidation",
    EBdigiCollection = cms.InputTag("simEcalDigis","ebDigis"),
    EBuncalibrechitCollection = cms.InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEB"),
    verbose = cms.untracked.bool(False)
)




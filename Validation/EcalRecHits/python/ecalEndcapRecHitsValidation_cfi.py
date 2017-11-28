import FWCore.ParameterSet.Config as cms

ecalEndcapRecHitsValidation = DQMStep1Analyzer("EcalEndcapRecHitsValidation",
    EEdigiCollection = cms.InputTag("simEcalDigis","eeDigis"),
    EEuncalibrechitCollection = cms.InputTag("ecalMultiFitUncalibRecHit","EcalUncalibRecHitsEE"),
    verbose = cms.untracked.bool(False)
)




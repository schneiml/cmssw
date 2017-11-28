import FWCore.ParameterSet.Config as cms

mbueAndqcdValidation = DQMStep1Analyzer("MBUEandQCDValidation",
    hepmcCollection = cms.InputTag("generatorSmeared"),
    genChjetsCollection = cms.InputTag("chargedak4GenJets",""),
    genjetsCollection = cms.InputTag("ak4GenJets",""),
    verbosity = cms.untracked.uint32(0),
    UseWeightFromHepMC = cms.bool(True)
)

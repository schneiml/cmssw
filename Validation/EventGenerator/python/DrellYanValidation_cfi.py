import FWCore.ParameterSet.Config as cms

drellYanValidation = DQMStep1Analyzer("DrellYanValidation",
    hepmcCollection = cms.InputTag("generatorSmeared"),
    decaysTo = cms.int32(11),
    name = cms.string("Electrons"),
    UseWeightFromHepMC = cms.bool(True)
)

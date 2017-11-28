import FWCore.ParameterSet.Config as cms

basicHepMCValidation = DQMStep1Analyzer("BasicHepMCValidation",
    hepmcCollection = cms.InputTag("generatorSmeared"),
    UseWeightFromHepMC = cms.bool(True)
)

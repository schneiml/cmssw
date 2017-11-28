import FWCore.ParameterSet.Config as cms

basicHepMCHeavyIonValidation = DQMStep1Analyzer("BasicHepMCHeavyIonValidation",
    hepmcCollection = cms.InputTag("generatorSmeared"),
    UseWeightFromHepMC = cms.bool(True)
)

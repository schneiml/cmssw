import FWCore.ParameterSet.Config as cms

duplicationChecker = DQMStep1Analyzer("DuplicationChecker",
    hepmcCollection = cms.InputTag("generatorSmeared"),
    searchForLHE = cms.bool(False),
    UseWeightFromHepMC = cms.bool(True)
)

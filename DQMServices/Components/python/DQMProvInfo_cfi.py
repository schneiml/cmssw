import FWCore.ParameterSet.Config as cms


dqmProvInfo = DQMStep1Analyzer("DQMProvInfo",
    subSystemFolder = cms.untracked.string('Info'),
    provInfoFolder = cms.untracked.string('ProvInfo'),
    runType = cms.untracked.string("No Run Type selected")
)

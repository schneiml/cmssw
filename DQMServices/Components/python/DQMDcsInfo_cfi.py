import FWCore.ParameterSet.Config as cms


dqmDcsInfo = DQMStep1Analyzer("DQMDcsInfo",
    subSystemFolder = cms.untracked.string('Info'),
    dcsInfoFolder = cms.untracked.string('DcsInfo')
)

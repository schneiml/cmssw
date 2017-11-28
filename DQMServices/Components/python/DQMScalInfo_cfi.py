import FWCore.ParameterSet.Config as cms

dqmscalInfo = DQMStep1Analyzer("DQMScalInfo",
    dqmScalFolder = cms.untracked.string('Scal')
)

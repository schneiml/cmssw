import FWCore.ParameterSet.Config as cms

dtScalerInfoMonitor = DQMStep1Analyzer("DTScalerInfoTask",
    inputTagScaler = cms.untracked.InputTag('scalersRawToDigi'),
)



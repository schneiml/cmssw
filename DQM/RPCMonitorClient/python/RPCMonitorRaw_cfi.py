import FWCore.ParameterSet.Config as cms


rpcMonitorRaw = DQMStep1Analyzer("RPCMonitorRaw",
  watchedErrors = cms.untracked.vint32(8,9),
  rpcRawDataCountsTag = cms.InputTag('rpcunpacker')
)

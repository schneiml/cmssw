import FWCore.ParameterSet.Config as cms

rpcFEDIntegrity = DQMStep1Analyzer("RPCFEDIntegrity",
                                 RPCPrefixDir =  cms.untracked.string('RPC/FEDIntegrity'),
                                 RPCRawCountsInputTag = cms.untracked.InputTag('muonRPCDigis'),
                                 NumberOfFED = cms.untracked.int32(3)
                                 )



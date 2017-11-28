import FWCore.ParameterSet.Config as cms
CentralitypADQM = DQMStep1Analyzer("CentralitypADQM", 
                                 centralitycollection = cms.InputTag("pACentrality"),
                                 vertexcollection = cms.InputTag("offlinePrimaryVertices")
                                 )

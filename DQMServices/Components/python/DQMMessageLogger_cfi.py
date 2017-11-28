
import FWCore.ParameterSet.Config as cms


DQMMessageLogger = DQMStep1Analyzer("DQMMessageLogger",
                             Categories = cms.vstring(),
                             Directory = cms.string("MessageLogger")
                             )



import FWCore.ParameterSet.Config as cms

hgcalDigiValidationEE = DQMStep1Module('HGCalDigiValidation',
                                       DetectorName  = cms.string("HGCalEESensitive"),
                                       DigiSource    = cms.InputTag("mix", "HGCDigisEE"),
                                       ifHCAL        = cms.bool(False),
                                       Verbosity     = cms.untracked.int32(0),
                                       SampleIndx    = cms.untracked.int32(0)
                                       )

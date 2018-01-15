import FWCore.ParameterSet.Config as cms

hgcalRecHitValidationEE = DQMStep1Module('HGCalRecHitValidation',
                                         DetectorName = cms.string("HGCalEESensitive"),
                                         RecHitSource = cms.InputTag("HGCalRecHit", "HGCEERecHits"),
                                         ifHCAL       = cms.bool(False),
                                         Verbosity    = cms.untracked.int32(0)
                                         
                                         )

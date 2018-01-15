import FWCore.ParameterSet.Config as cms

from DQMServices.Core.DQMEDAnalyzer import DQMEDAnalyzer
hgcalRecHitValidationEE = DQMEDAnalyzer('HGCalRecHitValidation',
                                         DetectorName = cms.string("HGCalEESensitive"),
                                         RecHitSource = cms.InputTag("HGCalRecHit", "HGCEERecHits"),
                                         ifHCAL       = cms.bool(False),
                                         Verbosity    = cms.untracked.int32(0)
                                         
                                         )

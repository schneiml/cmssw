import FWCore.ParameterSet.Config as cms

calotowersAnalyzer = DQMStep1Analyzer("CaloTowersAnalyzer",
    outputFile               = cms.untracked.string(''),
    CaloTowerCollectionLabel = cms.untracked.InputTag('towerMaker'),
    hcalselector             = cms.untracked.string('all'),
    useAllHistos             = cms.untracked.bool(False)
)


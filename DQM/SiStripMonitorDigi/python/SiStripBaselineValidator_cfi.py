import FWCore.ParameterSet.Config as cms

SiStripBaselineValidator = DQMStep1Analyzer("SiStripBaselineValidator",
    srcProcessedRawDigi =  cms.InputTag('siStripZeroSuppression','VirginRaw')
)

import FWCore.ParameterSet.Config as cms

hiBasicGenTest = DQMStep1Analyzer("HiBasicGenTest",
                                generatorLabel = cms.InputTag('generatorSmeared'),
                                outputFile = cms.string('')
)

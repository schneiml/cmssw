import FWCore.ParameterSet.Config as cms

#
# This object is used to make changes for different running scenarios
#
from Configuration.StandardSequences.Eras import eras

Phase1SiPixelRawDataErrorSource = cms.EDAnalyzer("Phase1SiPixelRawDataErrorSource",
    TopFolderName = cms.string('Pixel'),
    src = cms.InputTag("siPixelDigis"),
    outputFile = cms.string('Pixel_DQM_Error.root'),
    saveFile = cms.untracked.bool(False),
    isPIB = cms.untracked.bool(False),
    slowDown = cms.untracked.bool(False),
    reducedSet = cms.untracked.bool(False),
    modOn = cms.untracked.bool(True),
    ladOn = cms.untracked.bool(False),
    bladeOn = cms.untracked.bool(False),
    isUpgrade = cms.untracked.bool(True)
)


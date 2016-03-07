import FWCore.ParameterSet.Config as cms

# Pixel RawDataError Monitoring
#from DQM.Phase1SiPixelMonitorRawData.Phase1SiPixelMonitorRawData_cfi import * 
#Phase1SiPixelRawDataErrorSource.saveFile = False
#Phase1SiPixelRawDataErrorSource.isPIB = False
#Phase1SiPixelRawDataErrorSource.slowDown = False
#Phase1SiPixelRawDataErrorSource.reducedSet = False

# Pixel Digi Monitoring
from DQM.Phase1SiPixelMonitorDigi.Phase1SiPixelMonitorDigi_cfi import *
Phase1SiPixelDigiSource.saveFile = False
Phase1SiPixelDigiSource.isPIB = False
Phase1SiPixelDigiSource.slowDown = False


phase1SiPixelOfflineDQM_source = cms.Sequence(Phase1SiPixelDigiSource)

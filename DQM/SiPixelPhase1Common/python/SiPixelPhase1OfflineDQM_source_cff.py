import FWCore.ParameterSet.Config as cms

from DQM.SiPixelPhase1Common.MessageLogger_cfi import *

# Pixel Digi Monitoring
from DQM.SiPixelPhase1Digis.SiPixelPhase1Digis_cfi import *

siPixelPhase1OfflineDQM_source = cms.Sequence(SiPixelPhase1Digis)

import FWCore.ParameterSet.Config as cms

from DQM.SiPixelPhase1Common.HistogramManager_cfi import *

# Configure Phase1 DQM for Phase0 data
SiPixelPhase1Geometry.n_inner_ring_blades = 24 # no outer ring

# Pixel Digi Monitoring
from DQM.SiPixelPhase1Digis.SiPixelPhase1Digis_cfi import *
SiPixelPhase1DigisAnalyzer.src = cms.InputTag("siPixelDigis") # adapt for real data

# Cluster (track-independent) monitoring
from DQM.SiPixelPhase1Clusters.SiPixelPhase1Clusters_cfi import *

PerModule.enabled = True

siPixelPhase1OnlineDQM_source = cms.Sequence(SiPixelPhase1DigisAnalyzer
                                            + SiPixelPhase1ClustersAnalyzer
                                            )

siPixelPhase1OnlineDQM_harvesting = cms.Sequence(SiPixelPhase1DigisHarvester 
                                                + SiPixelPhase1ClustersHarvester
                                                )

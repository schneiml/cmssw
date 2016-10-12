import FWCore.ParameterSet.Config as cms

process = cms.Process("PIXELDQMLIVE")

process.MessageLogger = cms.Service("MessageLogger",
    debugModules = cms.untracked.vstring(),
    cout = cms.untracked.PSet(threshold = cms.untracked.string('ERROR')),
    destinations = cms.untracked.vstring('cout')
)

QTestfile = 'DQM/SiPixelMonitorClient/test/sipixel_qualitytest_config.xml'
#----------------------------
# Event Source
#-----------------------------
# for live online DQM in P5
process.load("DQM.Integration.config.inputsource_cfi")

# for testing in lxplus
#process.load("DQM.Integration.config.fileinputsource_cfi")

##
#----------------------------
# DQM Environment
#-----------------------------
process.load("DQMServices.Components.DQMEnvironment_cfi")

#----------------------------
# DQM Live Environment
#-----------------------------
process.load("DQM.Integration.config.environment_cfi")
process.dqmEnv.subSystemFolder    = "Pixel"
process.dqmSaver.tag = "Pixel"

process.DQMStore.referenceFileName = '/dqmdata/dqm/reference/pixel_reference_pp.root'
if (process.runType.getRunType() == process.runType.hi_run):
    process.DQMStore.referenceFileName = '/dqmdata/dqm/reference/pixel_reference_hi.root'

if (process.runType.getRunType() == process.runType.cosmic_run):
    process.DQMStore.referenceFileName = '/dqmdata/dqm/reference/pixel_reference_cosmic.root'
    process.source.SelectEvents = cms.untracked.vstring('HLT*SingleMu*')

#-----------------------------
# Magnetic Field
#-----------------------------
# 3.8T field
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')

#-------------------------------------------------
# GEOMETRY
#-------------------------------------------------
process.load("Configuration.StandardSequences.GeometryRecoDB_cff")

#-------------------------------------------------
# GLOBALTAG
#-------------------------------------------------
# Condition for P5 cluster
process.load("DQM.Integration.config.FrontierCondition_GT_cfi")
# Condition for lxplus: change and possibly customise the GT
#from Configuration.AlCa.GlobalTag import GlobalTag as gtCustomise
#process.GlobalTag = gtCustomise(process.GlobalTag, 'auto:run2_data', '')

#-----------------------
#  Reconstruction Modules
#-----------------------
# Real data raw to digi
process.load("EventFilter.SiPixelRawToDigi.SiPixelRawToDigi_cfi")
process.siPixelDigis.IncludeErrors = True

# Local Reconstruction
process.load("RecoLocalTracker.SiPixelClusterizer.SiPixelClusterizer_cfi")

#----------------------
# Pilot Blade
#----------------------
process.load('DPGAnalysis.PilotBladeStudy.PilotBladeSetup_cfi')

# process.CablingMapDBReader.connect = 'sqlite_file:../../../../DPGAnalysis/PilotBladeStudy/test/SiPixelFedCablingMap_FED1240_v2.db'
process.CablingMapDBReader.toGet[0].tag = cms.string('SiPixelFedCablingMap_FED1240_v2') 

process.GlobalTag.toGet = cms.VPSet(
         cms.PSet(record = cms.string('TrackerAlignmentRcd'),
                  tag =  cms.string('Alignments'),
                  connect = cms.string('sqlite_file:../../../../DPGAnalysis/PilotBladeStudy/test/tracker_alignment_80X_dataRun2_Prompt_v8.db')
                  ),
         cms.PSet(record = cms.string('TrackerAlignmentErrorExtendedRcd'),
                  tag =  cms.string('AlignmentErrorsExtended'),
                  connect = cms.string('sqlite_file:../../../../DPGAnalysis/PilotBladeStudy/test/tracker_alignment_80X_dataRun2_Prompt_v8.db')
                  ),
         cms.PSet(record = cms.string('TrackerSurfaceDeformationRcd'),
                  tag =  cms.string('AlignmentSurfaceDeformations'),
                  connect = cms.string('sqlite_file:../../../../DPGAnalysis/PilotBladeStudy/test/tracker_alignment_80X_dataRun2_Prompt_v8.db')
                  )
         )

# TODO: where is this info added?
process.siPixelDigis.UseQualityInfo = cms.bool(True)

#Pilot Blade Digis
process.PBDigis = cms.EDProducer("SiPixelRawToDigi",
  InputLabel = cms.InputTag("rawDataCollector"),
  CablingMapLabel =  cms.string("pilotBlade"),
  UsePhase1 = cms.bool(False),
  UsePilotBlade = cms.bool(True),
  UseQualityInfo = cms.bool(False),
  IncludeErrors = cms.bool(True),
  UserErrorList = cms.vint32(25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40)
)
#Pilot Blade Clusters
from CondTools.SiPixel.SiPixelGainCalibrationService_cfi import *
process.PBClusters = cms.EDProducer("SiPixelClusterProducer",
  SiPixelGainCalibrationServiceParameters,
  src = cms.InputTag("PBDigis"),
  ChannelThreshold = cms.int32(1000),
  MissCalibrate = cms.untracked.bool(False),
  SplitClusters = cms.bool(False),
  VCaltoElectronGain = cms.int32(65),
  VCaltoElectronOffset = cms.int32(-414),                          
  payloadType = cms.string('Offline'),
  SeedThreshold = cms.int32(1000),
  ClusterThreshold = cms.double(4000.0),
  maxNumberOfClusters = cms.int32(-1),
)

#Pilot Blade RecHits
# TODO: needs (specialized) reco
process.PBRecHits = cms.EDProducer("SiPixelRecHitConverter",
  src = cms.InputTag("PBClusters"),
  CPE = cms.string('PixelCPEGeneric'),
  VerboseLevel = cms.untracked.int32(0),
)

# Phase1 DQM
process.load("DQM.SiPixelPhase1Config.SiPixelPhase1OnlineDQM_cff")

process.SiPixelPhase1Geometry.CablingMapLabel = "pilotBlade"
process.siPixelDigis.InputLabel   = cms.InputTag("rawDataCollector")
process.SiPixelPhase1ClustersAnalyzer.src = "PBClusters"
process.SiPixelPhase1DigisAnalyzer.src = "PBDigis"

#--------------------------
# Service
#--------------------------
process.AdaptorConfig = cms.Service("AdaptorConfig")

#--------------------------
# Filters
#--------------------------
# HLT Filter
# 0=random, 1=physics, 2=calibration, 3=technical
process.hltTriggerTypeFilter = cms.EDFilter("HLTTriggerTypeFilter",
    SelectedTriggerType = cms.int32(1)
)

#--------------------------
# Scheduling
#--------------------------
process.DQMmodules = cms.Sequence(process.dqmEnv*process.dqmSaver)

process.p = cms.Path(
    process.DQMmodules
  * process.PBDigis
  * process.PBClusters
  * process.siPixelPhase1OnlineDQM_source
  * process.siPixelPhase1OnlineDQM_harvesting
)
    
### process customizations included here
from DQM.Integration.config.online_customizations_cfi import *
process = customise(process)

print "Running with run type = ", process.runType.getRunType()

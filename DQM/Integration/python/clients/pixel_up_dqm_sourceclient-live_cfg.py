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
from DQM.SiPixelPhase1Common.HistogramManager_cfi import *
DefaultHisto.enabled = False
process.load("DQM.SiPixelPhase1Config.SiPixelPhase1OnlineDQM_cff")

process.SiPixelPhase1Geometry.CablingMapLabel = "pilotBlade"
process.siPixelDigis.InputLabel   = cms.InputTag("rawDataCollector")
process.SiPixelPhase1ClustersAnalyzer.src = "PBClusters"
process.SiPixelPhase1DigisAnalyzer.src = "PBDigis"
process.SiPixelPhase1RawDataAnalyzer.src = "PBDigis"

# turn on and configure specific histograms

# digis
process.SiPixelPhase1DigisADC.enabled = True
process.SiPixelPhase1DigisADC.specs = cms.VPSet(
  StandardSpecification2DProfile
)

process.SiPixelPhase1DigisNdigis.enabled = True
process.SiPixelPhase1DigisNdigis.specs = cms.VPSet(
  StandardSpecification2DProfile_Num
)

process.SiPixelPhase1DigisNdigisPerFED.enabled = True
process.SiPixelPhase1DigisNdigisPerFED.specs = cms.VPSet(
	Specification().groupBy("FED/Event")
		   .reduce("COUNT")
		   .groupBy("FED")
		   .groupBy("", "EXTEND_Y")
		   .save(),
)

process.SiPixelPhase1DigisEvents.enabled = True
process.SiPixelPhase1DigisHitmap.enabled = True

# clusters
process.SiPixelPhase1ClustersCharge.enabled = True
process.SiPixelPhase1ClustersCharge.bookUndefined = False
process.SiPixelPhase1ClustersCharge.specs = cms.VPSet(
  StandardSpecification2DProfile,
  Specification(PerModule).groupBy("PXForward/PXDisk/DetId").save() 
)

process.SiPixelPhase1ClustersSize.enabled = True
process.SiPixelPhase1ClustersSize.bookUndefined = False
process.SiPixelPhase1ClustersSize.specs = cms.VPSet(
  StandardSpecification2DProfile,
  Specification(PerModule).groupBy("PXForward/PXDisk/DetId").save() 
)

process.SiPixelPhase1ClustersNClusters.enabled = True
process.SiPixelPhase1ClustersNClusters.bookUndefined = False
process.SiPixelPhase1ClustersNClusters.specs = cms.VPSet(
  StandardSpecification2DProfile_Num,
  Specification(PerModule).groupBy("PXForward/PXDisk/DetId/Event")
                          .reduce("COUNT")
                          .groupBy("PXForward/PXDisk/DetId").save() 
)

process.SiPixelPhase1ClustersPositionB.enabled = True
process.SiPixelPhase1ClustersPositionB.range_min = -80
process.SiPixelPhase1ClustersPositionB.range_max = 80
process.SiPixelPhase1ClustersPositionB.specs = cms.VPSet(
  Specification().groupBy("").save()
)

process.SiPixelPhase1ClustersPositionF.enabled = True
process.SiPixelPhase1ClustersPositionF.range_min = -15
process.SiPixelPhase1ClustersPositionF.range_max = 15
process.SiPixelPhase1ClustersPositionF.range_nbins = 1000
process.SiPixelPhase1ClustersPositionF.range_y_min = -15
process.SiPixelPhase1ClustersPositionF.range_y_max = 15
process.SiPixelPhase1ClustersPositionF.range_y_nbins = 1000
process.SiPixelPhase1ClustersPositionF.specs = cms.VPSet(
  Specification().groupBy("PXForward").save(),
  Specification().groupBy("PXForward/PXDisk").save()
)

# errors
process.SiPixelPhase1RawDataNErrors.enabled = True
process.SiPixelPhase1RawDataNErrors.specs = cms.VPSet(
  Specification().groupBy("PXForward/PXBlade").groupBy("PXForward", "EXTEND_X").save(),
  Specification().groupBy("FEDChannel").groupBy("", "EXTEND_X").save()
)

process.SiPixelPhase1RawDataTypeNErrors.enabled = True


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
    process.PBDigis
  * process.PBClusters
  * process.DQMmodules
  * process.siPixelPhase1OnlineDQM_source
  * process.siPixelPhase1OnlineDQM_harvesting
)
    
### process customizations included here
from DQM.Integration.config.online_customizations_cfi import *
process = customise(process)

print "Running with run type = ", process.runType.getRunType()

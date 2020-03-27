import FWCore.ParameterSet.Config as cms

# needed backend
from DQMServices.Core.DQMStore_cfg import *

# needed output
from DQMServices.Components.DQMEnvironment_cfi import *
dqmSaver.convention = 'Offline'
dqmSaver.workflow = '/Global/CMSSW_X_Y_Z/RECO'

dqmiooutput = cms.OutputModule("DQMRootOutputModule",
    fileName = cms.untracked.string("dqmio_endjob.root"),
    saveAtEndJob = cms.untracked.uint32(1)
)

DQMSaver = cms.Sequence(dqmSaver+dqmiooutput)

import FWCore.ParameterSet.Config as cms

from Calibration.EcalCalibAlgos.ecalPedestalPCLHarvester_cfi import ECALpedestalPCLHarvester
from DQMServices.Components.EDMtoMEConverter_cfi import *

EDMtoMEConvertEcalPedestals = EDMtoMEConverter.clone()
EDMtoMEConvertEcalPedestals.lumiInputTag = cms.InputTag("MEtoEDMConvertEcalPedestals", "MEtoEDMConverterLumi")
EDMtoMEConvertEcalPedestals.runInputTag = cms.InputTag("MEtoEDMConvertEcalPedestals", "MEtoEDMConverterRun")

DQMStore = cms.Service("DQMStore")

DQMInfoEcalPedestals = DQMStep1Analyzer("DQMEventInfo",
                                      subSystemFolder=cms.untracked.string('AlCaReco'),
                                      )

ALCAHARVESTEcalPedestals = cms.Sequence(EDMtoMEConvertEcalPedestals + DQMInfoEcalPedestals + ECALpedestalPCLHarvester)

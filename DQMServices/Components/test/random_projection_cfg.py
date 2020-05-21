import FWCore.ParameterSet.Config as cms

process = cms.Process("HARVESTING")
process.add_(cms.Service("DQMStore"))
process.RandomProjector = cms.EDProducer("RandomProjector",
  meprefix = cms.untracked.string(""),
  ndimensions = cms.untracked.int32(100))

process.source = cms.Source("DQMRootSource",
  fileNames = cms.untracked.vstring(),
  reScope = cms.untracked.string("LUMI"))

process.harvest = cms.Sequence(process.RandomProjector)
process.p = cms.Path(process.harvest)

#process.Tracer = cms.Service("Tracer")

process.source.fileNames = [
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/164734A3-78B5-E244-BD56-F5C873721E0D.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/43FBEB59-4BB4-764E-B43D-5CA8EA43597A.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/A54D10A9-2901-3C4F-96EA-06A33CEB482B.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/90155F14-F6A4-134A-8D50-65B4B8D07254.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/D576BB76-7C4E-854E-8B78-43F9D61C3AA3.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/BC7D824B-F348-0948-87A3-3E01FF738556.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/78298E39-94D8-8A45-AB56-92277F15E6ED.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/4269D1F8-D1B7-9B44-8091-8FBA3E40E8FB.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/D79B646A-3428-1547-A65A-D23E7D68ACD6.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/820E6F77-1C6B-7141-97FE-5B48C90EAF06.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/BA48FF32-BCA5-3341-A30C-F9DA16D40C74.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/6232C05B-C028-8A41-BFD1-6875ABDFB146.root',
#'/store/data/Run2017D/ZeroBias/DQMIO/09Aug2019_UL2017-v1/270000/BB317924-C925-474A-877A-CCCCE005458B.root',
]

import sys
if len(sys.argv) > 2:
  for name in sys.argv[2:]:
    process.source.fileNames.append(name)


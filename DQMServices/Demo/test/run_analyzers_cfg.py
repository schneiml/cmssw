import FWCore.ParameterSet.Config as cms

process = cms.Process("TEST")
process.DQMStore = cms.Service("DQMStore")
process.MessageLogger = cms.Service("MessageLogger")

process.load("DQMServices.Demo.test_cfi")
process.load("DQMServices.Demo.testone_cfi")
process.load("DQMServices.Demo.testonefillrun_cfi")
process.load("DQMServices.Demo.testonelumi_cfi")
process.load("DQMServices.Demo.testonelumifilllumi_cfi")
process.load("DQMServices.Demo.testglobal_cfi")
process.load("DQMServices.Demo.testlegacy_cfi")
process.load("DQMServices.Demo.testlegacyfillrun_cfi")
process.load("DQMServices.Demo.testlegacyfilllumi_cfi")
process.test_general = cms.Sequence(process.test 
                                  + process.testglobal)
process.test_one     = cms.Sequence(process.testone
                                  + process.testonefillrun)
process.test_legacy  = cms.Sequence(process.testonelumi + process.testonelumifilllumi
                                  + process.testlegacy + process.testlegacyfillrun + process.testlegacyfilllumi)

import FWCore.ParameterSet.VarParsing as VarParsing
parser = VarParsing.VarParsing('python')
one = VarParsing.VarParsing.multiplicity.singleton
int = VarParsing.VarParsing.varType.int
bool = VarParsing.VarParsing.varType.bool
string = VarParsing.VarParsing.varType.string
parser.register('nolegacy',             False, one, bool, "Don't run modules which block concurrent lumis.")
parser.register('noone',                False, one, bool, "Don't run any one modules.")
parser.register('firstLuminosityBlock', 1, one, int, "See EmptySource.")
parser.register('firstEvent',           1, one, int, "See EmptySource.")
parser.register('firstRun',             1, one, int, "See EmptySource.")
parser.register('numberEventsInRun',    100, one, int, "See EmptySource.")
parser.register('numberEventsInLuminosityBlock', 20, one, int, "See EmptySource.")
parser.register('nEvents',              100, one, int, "Total number of events.")
parser.register('nThreads',             1, one, int, "Number of threads and streams.")
parser.register('nConcurrent',          1, one, int, "Number of concurrent runs/lumis.")
parser.register('howmany',              1, one, int, "Number of MEs to book of each type.")
parser.register('outfile',              "dqm.root", one, string, "Output file name.")
parser.parseArguments()
args = parser


process.source = cms.Source("EmptySource", numberEventsInRun = cms.untracked.uint32(args.numberEventsInRun),
                            numberEventsInLuminosityBlock = cms.untracked.uint32(args.numberEventsInLuminosityBlock),
                            firstLuminosityBlock = cms.untracked.uint32(args.firstLuminosityBlock),
                            firstEvent = cms.untracked.uint32(args.firstEvent),
                            firstRun = cms.untracked.uint32(args.firstRun))

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(args.nEvents) )

process.options = cms.untracked.PSet(
  numberOfThreads = cms.untracked.uint32(args.nThreads),
  numberOfStreams = cms.untracked.uint32(args.nThreads),
  numberOfConcurrentLuminosityBlocks = cms.untracked.uint32(args.nConcurrent),
  # Must be one for now.
  numberOfConcurrentRuns = cms.untracked.uint32(1)
)

for mod in [process.test, process.testglobal, process.testone, process.testonefillrun, process.testonelumi, process.testonelumifilllumi, process.testlegacy, process.testlegacyfillrun, process.testlegacyfilllumi]:
  mod.howmany = args.howmany

if args.noone:
  process.p = cms.Path(process.test_general)
elif args.nolegacy:
  process.p = cms.Path(process.test_general + process.test_one)
else:
  process.p = cms.Path(process.test_general + process.test_one + process.test_legacy)

process.out = cms.OutputModule(
  "DQMRootOutputModule",
  fileName = cms.untracked.string(args.outfile),
  outputCommands = cms.untracked.vstring(
    'keep *'
  )
)

process.o = cms.EndPath(process.out)


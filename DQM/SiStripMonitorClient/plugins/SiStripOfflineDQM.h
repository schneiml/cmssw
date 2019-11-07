#ifndef SiStripMonitorClient_SiStripOfflineDQM_h
#define SiStripMonitorClient_SiStripOfflineDQM_h
// -*- C++ -*-
//
// Package:     SiStripMonitorClient
// Class  :     SiStripOfflineDQM
//
/**\class SiStripOfflineDQM SiStripOfflineDQM.h
   DQM/SiStripMonitorCluster/interface/SiStripOfflineDQM.h

   Description:
   DQM class to perform Summary creation Quality Test on a merged Root file
   after CAF processing
   Usage:
   <usage>

*/
//
// Original Author:  Samvel Khalatyan (ksamdev at gmail dot com)
//         Created:  Wed Oct 5 16:47:14 CET 2006
//


#include "DQMServices/Core/interface/DQMEDHarvester.h"
#include "DQM/SiStripMonitorClient/interface/SiStripActionExecutor.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <string>
#include <map>
#include <TTree.h>

class SiStripDetCabling;

// TODO: this module does many things that are varying degrees of "illegal".
// It is a legacy module; the fact that it is based on DQMEDHarvester does not 
// change that. It uses the DQMEDHarvester base clase only to get approximately
// sane order-of-execution relative to other modules (QualityTester).
class SiStripOfflineDQM : public DQMEDHarvester {
public:
  SiStripOfflineDQM(edm::ParameterSet const& ps);

private:
  void beginJob() override;
  void beginRun(edm::Run const& run, edm::EventSetup const& eSetup) override;
  void dqmEndLuminosityBlock(DQMStore::IBooker& , DQMStore::IGetter&, edm::LuminosityBlock const& lumiSeg, edm::EventSetup const& iSetup) override;
  void dqmEndRun(DQMStore::IBooker& , DQMStore::IGetter&, edm::Run const& run, edm::EventSetup const& eSetup) override;
  void dqmEndJob(DQMStore::IBooker& , DQMStore::IGetter&) override;

  void checkTrackerFEDs(edm::Event const& e);
  bool openInputFile(DQMStore& dqm_store);

  edm::ParameterSet const configPar_;

  SiStripActionExecutor actionExecutor_;

  bool usedWithEDMtoMEConverter_;
  bool createSummary_;
  bool const createTkInfoFile_;
  std::string const inputFileName_;
  std::string const outputFileName_;
  int globalStatusFilling_;
  bool trackerFEDsFound_;
  bool printFaultyModuleList_;
  TTree* tkinfoTree_{nullptr};
};
#endif

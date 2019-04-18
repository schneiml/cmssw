#ifndef HarvestingAnalyzer_h
#define HarvestingAnalyzer_h

/** \class HarvestingAnalyzer
 *  
 *  Class to perform operations on MEs after EDMtoMEConverter
 *
 *  \author M. Strang SUNY-Buffalo
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

//DQM services
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <iostream>
#include <cstdlib>
#include <string>
#include <memory>
#include <vector>

#include "TString.h"

class HarvestingAnalyzer : public edm::EDAnalyzer
{
  
 public:

  explicit HarvestingAnalyzer(const edm::ParameterSet&);
  ~HarvestingAnalyzer() override;
  void beginJob() override;
  void endJob() override;  
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void beginRun(const edm::Run&, const edm::EventSetup&) override;
  void endRun(const edm::Run&, const edm::EventSetup&) override;

  
private:
  std::string fName;
  int verbosity;
  std::unique_ptr<DQMStore> dbe;

};

#endif

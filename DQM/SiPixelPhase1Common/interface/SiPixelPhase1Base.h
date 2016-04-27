#ifndef SiPixel_DefaultTemplates_h
#define SiPixel_DefaultTemplates_h
// 
// This defines two classes, one that has to be extended to make a new plugin,
// and one that can be used as-is for the Harvesting.
//
// As with the entire SiPixelPhase1Common framework, you do not have to use
// this but can still use the other helpers. However, the HistogramManager
// needs to run in step1 and step2 of the DQM, and need to have exactly the
// same spec to work properly. This has to be guranteed by the configuration.
//
// Original Author: Marcel Schneider
//

#include "DQMServices/Core/interface/DQMEDAnalyzer.h"
#include "DQMServices/Core/interface/DQMEDHarvester.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQM/SiPixelPhase1Common/interface/HistogramManager.h"
#include "DQM/SiPixelPhase1Common/interface/SiPixelPhase1Enum.h"

#include <vector>
#include <iostream>

// used as a mixin for Analyzer and Harvester.
class HistogramManagerHolder {
  public:
  HistogramManagerHolder(const edm::ParameterSet& iConfig)
    : geometryInterface(iConfig.getParameter<edm::ParameterSet>("geometry")) {
    auto histograms = iConfig.getParameter<edm::ParameterSet>("histograms");
    std::vector<std::string> names;
    if (this->_enum_max() == 0) { // This is used for default harvesters
      histograms.getParameterSetNames(names);
      std::cout << "+++ HistogramManagerHolder: using config order\n";
    } else { // for specialized ones, make sure the order is correct.
      std::cout << "+++ HistogramManagerHolder: using enum order\n";
      for (unsigned int i = 0; i < this->_enum_max(); i++) 
        names.push_back(this->_tostring(i));
      //TODO: maybe push the others as well
    }
    for (auto& name : names) {
      std::cout << "+++ HistogramManagerHolder: putting " << name << "\n";
      histo.emplace_back(HistogramManager(histograms.getParameter<edm::ParameterSet>(name), geometryInterface));
    }
  };

  SIPIXEL_PHASE1_ENUM();

  protected:
  std::vector<HistogramManager> histo;
  GeometryInterface geometryInterface;
};

// This is the base class your plugin may derive from. You are not required to
// use it but if you just need some normal HistogramManager this should be perfect.
class SiPixelPhase1Base : public DQMEDAnalyzer, public HistogramManagerHolder {
  public:
  SiPixelPhase1Base(const edm::ParameterSet& iConfig) 
    : DQMEDAnalyzer(), HistogramManagerHolder(iConfig) {};

  // You should analyze something, and call histoman.fill(...).
  //void analyze(edm::Event const& e, edm::EventSetup const& eSetup);

  // This booking is usually fine.
  void bookHistograms(DQMStore::IBooker& iBooker, edm::Run const& run, edm::EventSetup const& iSetup) {
    for (HistogramManager& histoman : histo)
      histoman.book(iBooker, iSetup);
  };

  virtual ~SiPixelPhase1Base() {};
};

// This wraps the Histogram Managers into a DQMEDHarvester. It 
// provides sane default implementations, so most plugins don't care about this.
// However, you have to instantiate one with the same config as your Analyzer 
// to get the Harvesting one.
// For custom harvesting, you have to derive from this one. 
class SiPixelPhase1Harvester : public DQMEDHarvester, public HistogramManagerHolder {
  public:
  SiPixelPhase1Harvester(const edm::ParameterSet& iConfig) 
    : DQMEDHarvester(), HistogramManagerHolder(iConfig) {};

  void dqmEndLuminosityBlock(DQMStore::IBooker& iBooker, DQMStore::IGetter& iGetter, edm::LuminosityBlock const& lumiBlock, edm::EventSetup const& eSetup) {
    for (HistogramManager& histoman : histo)
      histoman.executeHarvestingOnline(iBooker, iGetter, eSetup);
  };
  void dqmEndJob(DQMStore::IBooker& iBooker, DQMStore::IGetter& iGetter) {
    for (HistogramManager& histoman : histo)
      histoman.executeHarvestingOffline(iBooker, iGetter);
  };
};
#endif

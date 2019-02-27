#ifndef DQMSERIVES_CORE_DQMEDANALYZER_H
#define DQMSERIVES_CORE_DQMEDANALYZER_H

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/one/EDProducer.h"

#include "DQMServices/Core/interface/DQMStore.h"

namespace one {

  struct DQMLuminosityBlockElements {};

  template <typename... T>
  class DQMEDAnalyzer : public edm::one::EDProducer<//edm::Accumulator,
                                                    //edm::EndRunProducer,
                                                    //edm::one::WatchRuns, 
                                                    T...> {
  public:
    // things that others implement
    virtual void analyze(edm::Event const&, edm::EventSetup const&) {};
    virtual void bookHistograms(DQMStore::IBooker &, edm::Run const&, edm::EventSetup const&) {};
    virtual void dqmBeginRun(edm::Run const&, edm::EventSetup const&) {};
    virtual void beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& setup) {};
    virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&) {};
    virtual void endRun(const edm::Run&, const edm::EventSetup&) {};



    // things we need to implement
    virtual void produce(edm::Event&, edm::EventSetup const&) final {};


    // formalities
    virtual ~DQMEDAnalyzer() noexcept (false) {};

      
  };
};


using DQMEDAnalyzer = one::DQMEDAnalyzer<>;

#endif


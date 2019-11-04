#ifndef DQMServices_Core_DQMEDAnalyzer_h
#define DQMServices_Core_DQMEDAnalyzer_h

#include "FWCore/Framework/interface/stream/makeGlobal.h"

namespace dqm {
  namespace reco {
    struct DQMEDAnalyzerGlobalCache;
  };  // namespace reco
}  // namespace dqm

namespace edm::stream::impl {
  template <typename T>
  T* makeStreamModule(edm::ParameterSet const& iPSet, dqm::reco::DQMEDAnalyzerGlobalCache const* global) {
    return new T(iPSet);
  }
}  // namespace edm::stream::impl

#include "DQMServices/Core/interface/DQMStore.h"

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/EDPutToken.h"
#include "DataFormats/Histograms/interface/DQMToken.h"

namespace dqm {
  namespace reco {
    struct DQMEDAnalyzerGlobalCache {
      mutable std::mutex lock_;
      mutable DQMStore* master_;
      mutable edm::EDPutTokenT<DQMToken> lumiToken_;
      mutable edm::EDPutTokenT<DQMToken> runToken_;
    };
  };  // namespace reco
}  // namespace dqm

class DQMEDAnalyzer : public edm::stream::EDProducer<
                          // We use a lot of edm features. We need all the caches to get the global
                          // transitions, where we can pull the (shared) MonitorElements out of the
                          // DQMStores.
                          edm::GlobalCache<dqm::reco::DQMEDAnalyzerGlobalCache>,
                          edm::EndLuminosityBlockProducer,
                          edm::EndRunProducer,
                          // This feature is essentially made for DQM and required to get per-event calls.
                          edm::Accumulator> {
protected:
  dqm::reco::DQMStore* dqmstore_;
  edm::EDPutTokenT<DQMToken> lumiToken_;
  edm::EDPutTokenT<DQMToken> runToken_;

public:
  typedef dqm::reco::DQMStore DQMStore;
  typedef dqm::reco::MonitorElement MonitorElement;

  // The following EDM methods are listed (as much as possible) in the order
  // that edm calls them in.

  static std::unique_ptr<dqm::reco::DQMEDAnalyzerGlobalCache> initializeGlobalCache(edm::ParameterSet const&) {
    return std::make_unique<dqm::reco::DQMEDAnalyzerGlobalCache>();
  }

  DQMEDAnalyzer() {
    lumiToken_ = produces<DQMToken, edm::Transition::EndLuminosityBlock>("DQMGenerationRecoLumi");
    runToken_ = produces<DQMToken, edm::Transition::EndRun>("DQMGenerationRecoRun");
  }

  // TODO: this is overridden in subsystem code, make sure that is safe.
  virtual void beginJob(){};

  void beginStream(edm::StreamID id) {
    dqmstore_ = edm::Service<DQMStore>().operator->();

    auto lock = std::scoped_lock(globalCache()->lock_);
    if (globalCache()->runToken_.isUninitialized()) {
      globalCache()->lumiToken_ = lumiToken_;
      globalCache()->runToken_ = runToken_;
      globalCache()->master_ = dqmstore_;
    }
  }

  void beginRun(edm::Run const& run, edm::EventSetup const& setup) {
    dqmBeginRun(run, setup);
    // Booking can happen while another stream is already filling. As long as
    // the booking code is sane, this is safe and correct; however, it would be
    // cleaner to avoid re-configuring existing MEs.
    // TODO: implement a way to prevent re-configuring MEs, by having users
    // pass in a macro that only gets called when a ME is newly booked.
    // TODO: what about re-booking semantics?
    // TODO: need to thread-in module IDs again, or use local DQMStores.
    dqmstore_->bookTransaction(
        [this, &run, &setup](DQMStore::IBooker& booker) {
          booker.cd();
          this->bookHistograms(booker, run, setup);
        },
        run.run(),
        this->moduleDescription().id(),
        /* canSaveByLumi */ true);
  }

  void beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& setup) {
    dqmstore_->enterLumi(lumi.run(), lumi.luminosityBlock());
    dqmBeginLuminosityBlock(lumi, setup);
  }

  void accumulate(edm::Event const& ev, edm::EventSetup const& es) { analyze(ev, es); }

  void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){};

  static void globalEndLuminosityBlockProduce(edm::LuminosityBlock& lumi,
                                              edm::EventSetup const& setup,
                                              LuminosityBlockContext const* context) {
    context->global()->master_->toProduct(lumi.run(), lumi.luminosityBlock());
    lumi.emplace<DQMToken>(context->global()->lumiToken_);
  }

  void endRun(edm::Run const& run, edm::EventSetup const& setup){};

  static void globalEndRunProduce(edm::Run& run, edm::EventSetup const& setup, RunContext const* context) {
    context->global()->master_->toProduct(run.run(), 0);
    run.emplace<DQMToken>(context->global()->runToken_);
  }

  static void globalEndJob(dqm::reco::DQMEDAnalyzerGlobalCache*){};

  // Methods to be implemented by the users
  virtual void bookHistograms(DQMStore::IBooker& i, edm::Run const&, edm::EventSetup const&) = 0;
  virtual void dqmBeginRun(edm::Run const& run, edm::EventSetup const& setup){};
  virtual void dqmBeginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){};
  virtual void analyze(edm::Event const&, edm::EventSetup const&){};
  // void endJob() final; // TODO: implemented in a few palces, not sure if ever called.
};

#endif  // DQMServices_Core_DQMEDAnalyzer_h

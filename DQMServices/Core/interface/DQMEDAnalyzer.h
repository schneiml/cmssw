#ifndef DQMServices_Core_DQMEDAnalyzer_h
#define DQMServices_Core_DQMEDAnalyzer_h

#include "FWCore/Framework/interface/stream/makeGlobal.h"

namespace dqm::reco {
  class DQMStoreGroup;
}

namespace edm::stream::impl {
  template <typename T>
  T* makeStreamModule(edm::ParameterSet const& iPSet, dqm::reco::DQMStoreGroup const*) {
    return new T(iPSet);
  }
}  // namespace edm::stream::impl

#include "DQMServices/Core/interface/DQMStore.h"

#include "FWCore/Framework/interface/stream/EDProducer.h"
#include "FWCore/Utilities/interface/EDPutToken.h"
#include "DataFormats/Histograms/interface/DQMToken.h"

namespace dqm {
  namespace reco {
    class DQMStoreGroup {
      // Use the internal type here so DQMstore can easily access it.
      mutable std::vector<std::shared_ptr<dqm::implementation::DQMStore<MonitorElement>>> dqmstores_;
      mutable std::mutex lock_;

    public:
      auto forId(int id) const {
        auto lock = std::scoped_lock(lock_);
        if (dqmstores_.size() < ((unsigned)id + 1)) {
          dqmstores_.resize(id + 1);
        }
        if (!dqmstores_[id]) {
          dqmstores_[id] = std::make_shared<DQMStore>();
        }
        // we thread-unsafely leak a reference here. This should only be used
        // after intialisation once this structure is no longer modified.
        dqmstores_[id]->setSiblings(&dqmstores_);
        return std::dynamic_pointer_cast<DQMStore>(dqmstores_[id]);
      }
    };

    class MonitorElementCollectionHolder {
      // This mostly exists to provide a const swap().
      // We swap MonitorElementCollection data a lot, the implementation is
      // std::vector::swap which is zero-copy and easy to reason about in
      // terms of ownership.
      mutable MonitorElementCollection mes;
      mutable std::mutex lock_;

    public:
      void swap(MonitorElementCollection& other) const {
        auto lock = std::scoped_lock(lock_);
        mes.swap(other);
      }
    };
  }  // namespace reco
}  // namespace dqm

class DQMEDAnalyzer : public edm::stream::EDProducer<
                          // We use a lot of edm features. We need all the caches to get the global
                          // transitions, where we can pull the (shared) MonitorElements out of the
                          // DQMStores.
                          edm::GlobalCache<dqm::reco::DQMStoreGroup>,
                          edm::EndLuminosityBlockProducer,
                          edm::LuminosityBlockSummaryCache<dqm::reco::MonitorElementCollectionHolder>,
                          edm::EndRunProducer,
                          edm::RunSummaryCache<dqm::reco::MonitorElementCollectionHolder>,
                          // This feature is essentially made for DQM and required to get per-event calls.
                          edm::Accumulator> {
  // TODO: seems we cannot use tokens here, since we have to put() from a
  // static context.
  //private:
  //  edm::EDPutTokenT<MonitorElementCollection> lumiToken_;
  //  edm::EDPutTokenT<MonitorElementCollection> runToken_;
protected:
  std::shared_ptr<dqm::reco::DQMStore> dqmstore_;

public:
  typedef dqm::reco::DQMStore DQMStore;
  typedef dqm::reco::MonitorElement MonitorElement;

  // The following EDM methods are listed (as much as possible) in the order
  // that edm calls them in.

  static std::unique_ptr<dqm::reco::DQMStoreGroup> initializeGlobalCache(edm::ParameterSet const&) {
    return std::make_unique<dqm::reco::DQMStoreGroup>();
  }

  DQMEDAnalyzer() {
    produces<MonitorElementCollection, edm::Transition::EndLuminosityBlock>("DQMGenerationRecoLumi");
    produces<MonitorElementCollection, edm::Transition::EndRun>("DQMGenerationRecoRun");
  }

  void beginStream(edm::StreamID id) { dqmstore_ = globalCache()->forId(id); }

  static std::shared_ptr<dqm::reco::MonitorElementCollectionHolder> globalBeginRunSummary(edm::Run const&,
                                                                                          edm::EventSetup const&,
                                                                                          RunContext const*) {
    return std::make_shared<dqm::reco::MonitorElementCollectionHolder>();
  }

  void beginRun(edm::Run const& run, edm::EventSetup const& setup) {
    dqmBeginRun(run, setup);
    // TODO: make sure booking does set the run number for each ME.
    // So if we see two runs, we can re-book without conflicts and can choose
    // to keep or drop the MEs from the previous run.
    // For multi-run harvesting, we should have a job-level granularity which
    // can also be used by default. Maybe we can make that a per-plugin option?
    this->bookHistograms(*dqmstore_, run, setup);
  }

  static std::shared_ptr<dqm::reco::MonitorElementCollectionHolder> globalBeginLuminosityBlockSummary(
      edm::LuminosityBlock const& lumi, edm::EventSetup const& setup, LuminosityBlockContext const* constex) {
    // this is just a placeholder, we swap in the real thing in endLuminosityBlockSummary.
    return std::make_shared<dqm::reco::MonitorElementCollectionHolder>();
  }

  void beginLuminosityBlock(edm::LuminosityBlock const& lumi, edm::EventSetup const& setup) {
    dqmBeginLuminosityBlock(lumi, setup);
  }

  void accumulate(edm::Event const& ev, edm::EventSetup const& es) { analyze(ev, es); }

  void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){};

  void endLuminosityBlockSummary(edm::LuminosityBlock const& lumi,
                                 edm::EventSetup const& setup,
                                 dqm::reco::MonitorElementCollectionHolder* data) const {};

  static void globalEndLuminosityBlockSummary(edm::LuminosityBlock const& lumi,
                                              edm::EventSetup const& setup,
                                              LuminosityBlockContext const* context,
                                              dqm::reco::MonitorElementCollectionHolder* data) {
    // TODO: all streams are done with the lumi, we can pull the MEs from the
    // DQMStores.
  }

  static void globalEndLuminosityBlockProduce(edm::LuminosityBlock& lumi,
                                              edm::EventSetup const& setup,
                                              LuminosityBlockContext const* context,
                                              dqm::reco::MonitorElementCollectionHolder const* data) {
    auto prod = std::make_unique<MonitorElementCollection>();
    data->swap(*prod);
    lumi.put(std::move(prod), "DQMGenerationRecoLumi");
  }

  void endRun(edm::Run const& run, edm::EventSetup const& setup){};

  void endRunSummary(edm::Run const& run,
                     edm::EventSetup const& setup,
                     dqm::reco::MonitorElementCollectionHolder* data) const {};

  static void globalEndRunSummary(edm::Run const& run,
                                  edm::EventSetup const& setup,
                                  RunContext const* context,
                                  dqm::reco::MonitorElementCollectionHolder const* data) {
    // TODO: all streams are done with the run, we can pull the MEs from the
    // DQMStores.
  }

  static void globalEndRunProduce(edm::Run& run,
                                  edm::EventSetup const& setup,
                                  RunContext const* context,
                                  dqm::reco::MonitorElementCollectionHolder const* data) {
    auto prod = std::make_unique<MonitorElementCollection>();
    data->swap(*prod);
    run.put(std::move(prod), "DQMGenerationRecoRun");
  }

  static void globalEndJob(dqm::reco::DQMStoreGroup*){};

  // Methods to be implemented by the users
  virtual void bookHistograms(DQMStore::IBooker& i, edm::Run const&, edm::EventSetup const&) = 0;
  virtual void dqmBeginRun(edm::Run const& run, edm::EventSetup const& setup){};
  virtual void dqmBeginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&){};
  virtual void analyze(edm::Event const&, edm::EventSetup const&){};
};

#endif  // DQMServices_Core_DQMEDAnalyzer_h

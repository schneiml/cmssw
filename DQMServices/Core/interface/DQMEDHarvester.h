#ifndef DQMSERVICES_CORE_DQMEDHARVESTER_H
#define DQMSERVICES_CORE_DQMEDHARVESTER_H

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/one/EDProducer.h"


class DQMEDHarvester: public edm::one::EDProducer<edm::Accumulator, // Accumulator allows us to not have produce()
                                                  edm::EndLuminosityBlockProducer,
                                                  edm::EndRunProducer,
                                                  edm::one::WatchLuminosityBlocks,
                                                  edm::one::WatchRuns,
                                                  edm::one::SharedResources>

{
public:
  ~DQMEDHarvester() override = default;

  void accumulate(edm::Event const& ev, edm::EventSetup const& es) final {};

  void beginRun(edm::Run const&, edm::EventSetup const&) override {};
  void endRun(edm::Run const&, edm::EventSetup const&) override {};
  void endRunProduce(edm::Run& run, edm::EventSetup const& setup) override {};

  void beginLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const&) final {};
  void endLuminosityBlock(edm::LuminosityBlock const &, edm::EventSetup const&) final {};
  void endLuminosityBlockProduce(edm::LuminosityBlock&, edm::EventSetup const&) final {};

  void endJob() final {};
  virtual void dqmEndLuminosityBlock(DQMStore::IBooker &, DQMStore::IGetter &, edm::LuminosityBlock const &, edm::EventSetup const&) {};
  virtual void dqmEndJob(DQMStore::IBooker &, DQMStore::IGetter &) = 0;

};

#endif

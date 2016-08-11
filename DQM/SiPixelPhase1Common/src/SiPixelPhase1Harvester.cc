// This is only to declare a plugin, the implementation is in the header. 
#include "DQM/SiPixelPhase1Common/interface/SiPixelPhase1Base.h"
#include "FWCore/Framework/interface/MakerMacros.h"

void SiPixelPhase1Harvester::dqmEndLuminosityBlock(DQMStore::IBooker& iBooker, DQMStore::IGetter& iGetter, edm::LuminosityBlock const& lumiBlock, edm::EventSetup const& eSetup) {
  for (HistogramManager& histoman : histo)
    histoman.executeHarvestingOnline(iBooker, iGetter, eSetup);
};
void SiPixelPhase1Harvester::dqmEndJob(DQMStore::IBooker& iBooker, DQMStore::IGetter& iGetter) {
  for (HistogramManager& histoman : histo)
    histoman.executeHarvestingOffline(iBooker, iGetter);
};

#include "DQM/EcalMonitorTasks/interface/IntegrityTask.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"

namespace ecaldqm {
  IntegrityTask::IntegrityTask() : DQWorkerTask() {}

  template <typename IDCollection>
  void IntegrityTask::runOnDetIdCollection(IDCollection const& _ids, Collections _collection) {
    // Collection is empty if there are no errors
    if (_ids.empty())
      return;

    MESet* set(nullptr);
    switch (_collection) {
      case kEBGainErrors:
      case kEEGainErrors:
        set = &MEs_.at("Gain");
        break;
      case kEBChIdErrors:
      case kEEChIdErrors:
        set = &MEs_.at("ChId");
        break;
      case kEBGainSwitchErrors:
      case kEEGainSwitchErrors:
        set = &MEs_.at("GainSwitch");
        break;
      default:
        return;
    }

    MESet& meTotal(MEs_.at("Total"));
    MESet& meTrendNErrors(MEs_.at("TrendNErrors"));

    std::for_each(_ids.begin(), _ids.end(), [&](typename IDCollection::value_type const& id) {
      set->fill(id);
      int dccid(dccId(id));
      meTotal.fill(dccid);

      meTrendNErrors.fill(double(timestamp_.iLumi), 1.);
    });
  }

  void IntegrityTask::runOnElectronicsIdCollection(EcalElectronicsIdCollection const& _ids, Collections _collection) {
    // Collection is empty if there are no errors
    if (_ids.empty())
      return;

    MESet* set(nullptr);
    switch (_collection) {
      case kTowerIdErrors:
        set = &MEs_.at("TowerId");
        break;
      case kBlockSizeErrors:
        set = &MEs_.at("BlockSize");
        break;
      default:
        return;
    }

    MESet& meTotal(MEs_.at("Total"));
    MESet& meTrendNErrors(MEs_.at("TrendNErrors"));

    std::for_each(_ids.begin(), _ids.end(), [&](EcalElectronicsIdCollection::value_type const& id) {
      set->fill(id);
      int dccid(id.dccId());
      double nCrystals(0.);
      std::vector<DetId> chIds(getElectronicsMap()->dccTowerConstituents(dccid, id.towerId()));
      if (dccid <= kEEmHigh + 1 || dccid >= kEEpLow + 1)
        nCrystals = chIds.size();
      else
        nCrystals = 25.;
      meTotal.fill(dccid, nCrystals);

      meTrendNErrors.fill(double(timestamp_.iLumi), nCrystals);
    });
  }

  DEFINE_ECALDQM_WORKER(IntegrityTask);
}  // namespace ecaldqm

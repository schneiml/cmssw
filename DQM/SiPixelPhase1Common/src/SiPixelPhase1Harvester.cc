// This is only to declare a plugin, the implementation is in the header. 
#include "DQM/SiPixelPhase1Common/interface/SiPixelPhase1Base.h"
#include "FWCore/Framework/interface/MakerMacros.h"

class SiPixelPhase1Harvester : public SiPixelPhase1HarvesterBase {
  public:
  SiPixelPhase1Harvester(edm::ParameterSet const& iConfig) 
   : SiPixelPhase1HarvesterBase(iConfig, this) {};
  SIPIXEL_PHASE1_ENUM();
};

DEFINE_FWK_MODULE(SiPixelPhase1Harvester);


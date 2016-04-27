#ifndef SiPixelPhase1Clusters_h 
#define SiPixelPhase1Clusters_h 
// -*- C++ -*-
// 
// Package:     SiPixelPhase1Clusters
// Class  :     SiPixelPhase1Clusters
//

// Original Author: Marcel Schneider

#include "DQM/SiPixelPhase1Common/interface/SiPixelPhase1Base.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

class SiPixelPhase1Clusters : public SiPixelPhase1Base {
  public:
  SIPIXEL_PHASE1_ENUM (
    CHARGE,
    SIZE,
    NCLUSTERS,
    EVENTRATE,
    POSITION_B,
    POSITION_F,
    SIZE_VS_ETA
  );

  explicit SiPixelPhase1Clusters(const edm::ParameterSet& conf);
  void analyze(const edm::Event&, const edm::EventSetup&);

  private:
  edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster> > srcToken_;
};

#endif

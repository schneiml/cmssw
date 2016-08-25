// -*- C++ -*-
//
// Package:     SiPixelPhase1RawData
// Class:       SiPixelPhase1RawData
//

// Original Author: Marcel Schneider

#include "DQM/SiPixelPhase1RawData/interface/SiPixelPhase1RawData.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"


SiPixelPhase1RawData::SiPixelPhase1RawData(const edm::ParameterSet& iConfig) :
  SiPixelPhase1Base(iConfig) 
{
  srcToken_ = consumes<DetSetVector<SiPixelRawDataError>>(iConfig.getParameter<edm::InputTag>("src"));
}

void SiPixelPhase1RawData::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  edm::Handle<DetSetVector<SiPixelRawDataError>> input;
  iEvent.getByToken(srcToken_, input);
  if (!input.isValid()) return;
  std::cout << "+++ have data.\n";

}

DEFINE_FWK_MODULE(SiPixelPhase1RawData);


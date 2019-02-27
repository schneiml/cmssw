/*
 * \file QualityTester.cc
 *
 * \author M. Zanetti - CERN PH
 *
 */

#include "DQMServices/Components/interface/QualityTester.h"
#include "DQMServices/ClientConfig/interface/QTestHandle.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"

#include <memory>
#include <cstdio>
#include <sstream>
#include <cmath>

using namespace edm;
using namespace std;

QualityTester::QualityTester(const ParameterSet& ps)
{
  prescaleFactor          = ps.getUntrackedParameter<int>("prescaleFactor", 1);
  getQualityTestsFromFile = ps.getUntrackedParameter<bool>("getQualityTestsFromFile", true);
  Label                   = ps.getUntrackedParameter<string>("label","");
  reportThreshold = ps.getUntrackedParameter<string>("reportThreshold", "");
  testInEventloop = ps.getUntrackedParameter<bool>("testInEventloop",false);
  qtestOnEndRun   = ps.getUntrackedParameter<bool>("qtestOnEndRun",true);
  qtestOnEndJob   = ps.getUntrackedParameter<bool>("qtestOnEndJob",false);
  qtestOnEndLumi  = ps.getUntrackedParameter<bool>("qtestOnEndLumi",false);
  verboseQT       = ps.getUntrackedParameter<bool>("verboseQT", true);

  bei = &*edm::Service<DQMStore>();

  qtHandler=new QTestHandle;

  // if you use this module, it's non-sense not to provide the QualityTests.xml
  if (getQualityTestsFromFile) {
    edm::FileInPath qtlist = ps.getUntrackedParameter<edm::FileInPath>("qtList");
    qtHandler->configureTests(FileInPath(qtlist).fullPath(), bei);
  }


  nEvents = 0;

}

void QualityTester::beginRun(const edm::Run& run , const edm::EventSetup& iSetup){

}

QualityTester::~QualityTester()
{
  delete qtHandler;
}

void QualityTester::analyze(const edm::Event& e, const edm::EventSetup& c) 
{
}

void QualityTester::endLuminosityBlock(LuminosityBlock const& lumiSeg, EventSetup const& context)
{
}

void QualityTester::endRun(const Run& r, const EventSetup& context){
  if (qtestOnEndRun) performTests();
}

void QualityTester::endJob(){
  if (qtestOnEndJob) performTests();
}

void QualityTester::performTests()
{
}

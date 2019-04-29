// -*- C++ -*-
//
// Package:    DQMServices/CoreROOT
// Class:      DQMReadFileExample
// 
/**\class DQMReadFileExample

Description: Simple example showing how to read MonitorElements from ROOT file

Implementation:
<Notes on implementation>
*/
//
//
//


// system include files
#include <memory>

#include <sstream>
#include <string>
#include <vector>

// user include files
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

//
// class declaration
//
class DQMReadFileExample : public edm::EDAnalyzer {
public:
  explicit DQMReadFileExample( const edm::ParameterSet& );
  ~DQMReadFileExample() override;
  
  void analyze( const edm::Event&, const edm::EventSetup& ) override;
  
  void endJob() override;

private:
  // ----------member data ---------------------------
  
  // back-end interface
  std::unique_ptr<DQMStore> dbe;
  
  // remove all MonitorElements and directories
/* almost unused */   void removeAll();
};

//
// constructors and destructor
//
DQMReadFileExample::DQMReadFileExample(const edm::ParameterSet& iConfig ) 
{
  // get hold of back-end interface
  dbe = std::make_unique<DQMStore>();

  std::string filename = iConfig.getUntrackedParameter<std::string>
    ("RootFileName", "test_playback.root");
  dbe->open(filename);
  dbe->showDirStructure();
  removeAll();

  bool overwrite = false; std::string pathname = "Collector/FU0/C1/C2";
  dbe->open(filename, overwrite, pathname);
  dbe->showDirStructure();
  removeAll();

  pathname = "Collector/FU0/C1/C3";
  dbe->open(filename, overwrite, pathname);
  dbe->showDirStructure();
  removeAll();
}



DQMReadFileExample::~DQMReadFileExample()
{
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  
}

// remove all MonitorElements and directories
/* almost unused */ void DQMReadFileExample::removeAll()
{
  // go to top directory
  dbe->cd();
  // remove MEs at top directory
  dbe->removeContents();
  // remove directory (including subdirectories recursively)
  if(dbe->dirExists("Collector"))
     dbe->rmdir("Collector");
  if(dbe->dirExists("Summary"))
  dbe->rmdir("Summary");
}

void DQMReadFileExample::endJob()
{
  dbe->showDirStructure();
  // dbe->save("test.root");  
}


//
// member functions
//

// ------------ method called to produce the data  ------------
void DQMReadFileExample::analyze(const edm::Event& iEvent, 
					 const edm::EventSetup& iSetup )
{

}

// define this as a plug-in
DEFINE_FWK_MODULE(DQMReadFileExample);


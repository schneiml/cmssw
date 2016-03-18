// -*- C++ -*-
//
// Package:     SiPixelPhase1Common
// Class  :     HistogramManager
//
#include "DQM/SiPixelPhase1Common/interface/HistogramManager.h"

#include <sstream>

// Geometry stuff
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"

HistogramManager::HistogramManager(const edm::ParameterSet& iconfig) :
  iConfig(iconfig),
  topFolderName(iconfig.getParameter<std::string>("TopFolderName"))
{
}

void HistogramManager::addSpec(SummationSpecification spec) {
  specs.push_back(spec);
  tables.push_back(Table());
}

SummationSpecificationBuilder HistogramManager::addSpec() {
  addSpec(SummationSpecification());
  return SummationSpecificationBuilder(specs[specs.size()-1]);
}

// note that this will be pretty hot. Ideally it should be malloc-free.
void HistogramManager::fill(double value, DetId sourceModule, const edm::Event *sourceEvent, int col, int row) {
  auto iq = GeometryInterface::InterestingQuantities{
              sourceModule, sourceEvent, col, row
	    };							    
  for (unsigned int i = 0; i < specs.size(); i++) {
    auto& s = specs[i];
    auto& t = tables[i];
    auto significantvalues = geometryInterface.extractColumns(s.steps[0].columns, iq);
    // TODO: more step1 steps must be excuted here. Step1 steps can be applied per-sample.
    // Step1 steps are those that have s.stage = SummationStep::STAGE1, and step[0].
    t[significantvalues].fill(value);
  }

  
}
  
void HistogramManager::book(DQMStore::IBooker& iBooker, edm::EventSetup const& iSetup) {
  if (!geometryInterface.loaded()) {
    geometryInterface.load(iSetup, iConfig);
  }

  // TODO: We need 2 passes, one to count the elements for EXTEND-ranges, one for actual booking.
  for (unsigned int i = 0; i < specs.size(); i++) {
    auto& s = specs[i];
    auto& t = tables[i];
    for (auto iq : geometryInterface.allModules()) {
      auto dimensions = this->dimensions;
      std::ostringstream name(this->name, std::ostringstream::ate);
      std::ostringstream dir("");
      std::ostringstream title(this->title, std::ostringstream::ate);
      std::ostringstream xlabel(this->xlabel, std::ostringstream::ate);
      std::ostringstream ylabel(this->ylabel, std::ostringstream::ate);
      auto significantvalues = geometryInterface.extractColumns(s.steps[0].columns, iq);
      for (SummationStep step : s.steps) {
	if (step.stage == SummationStep::STAGE1) {
	  //TODO: change labels, dimensionality, range, colums as fits
	}
      }

      AbstractHistogram& histo = t[significantvalues];
      if (histo.me) continue;

      for (auto c : s.steps[0].columns) {
	dir << c << "_" << std::hex << significantvalues[c] << "/";
      }

      iBooker.setCurrentFolder(topFolderName + "/" + dir.str());

      if (dimensions == 1) {
	title << ";" << xlabel.str();
      	histo.me = iBooker.book1D(name.str().c_str(), title.str().c_str(), 100, 0.0, 300.0);
      } else if (dimensions == 2) {
	title << ";" << xlabel.str() << ";" << ylabel.str();
	histo.me = iBooker.book2D(name.str().c_str(), title.str().c_str(), 100, 0.0, 300.0, 100, 0.0, 300.0);
      } else {
	std::cout << "Booking " << dimensions << " dimensions not supported.\n";
      }
    }
  }
}

void HistogramManager::executeHarvestingOnline(DQMStore::IBooker& iBooker, DQMStore::IGetter& iGetter, edm::EventSetup const& iSetup) {
  // this should also give us the GeometryInterface for offline, though it is a bit dirty and might explode.
  if (!geometryInterface.loaded()) {
    geometryInterface.load(iSetup, iConfig);
  }
  std::cout << "+++ HistogramManager: Step2 online\n";
}

void HistogramManager::executeHarvestingOffline(DQMStore::IBooker& iBooker, DQMStore::IGetter& iGetter) {
  std::cout << "+++ HistogramManager: Step2 offline\n";

  // This is esentially the booking code if step1, to reconstruct the ME names.
  // Once we have a name we load the ME and put it into the table.
  for (unsigned int i = 0; i < specs.size(); i++) {
    auto& s = specs[i];
    auto& t = tables[i];
    for (auto iq : geometryInterface.allModules()) {
      std::ostringstream name(this->name, std::ostringstream::ate);
      std::ostringstream dir(topFolderName + "/", std::ostringstream::ate);
      auto significantvalues = geometryInterface.extractColumns(s.steps[0].columns, iq);
      for (SummationStep step : s.steps) {
	if (step.stage == SummationStep::STAGE1) {
	  //TODO: change labels, dimensionality, range, colums as fits
	}
      }

      AbstractHistogram& histo = t[significantvalues];
      for (auto c : s.steps[0].columns) {
	dir << c << "_" << std::hex << significantvalues[c] << "/";
      }
      histo.me = iGetter.get(dir.str() + name.str());
      if (!histo.me) {
	std::cout << "+++ ME " << dir.str() + name.str() << " not found\n";
      } else {
	histo.th1 = histo.me->getTH1();
      }
    }
    
    // now execute step2.
    for (SummationStep step : s.steps) {
      if (step.stage == SummationStep::STAGE2) {

	//TODO: change labels, dimensionality, range, colums as fits
	
	// SAVE: traverse the table, book a ME for every TH1.
	// TODO: title and labels have to be recorded per-histo, in AbstractHistogram?
	if (step.type == SummationStep::SAVE) {
	  for (auto& e : t) {
	    if (e.second.me) continue; // if there is a ME already, nothing to do
	    GeometryInterface::Values vals = e.first;
	    std::ostringstream dir("");
	    for (auto c : s.steps[0].columns) {
	      if (vals.find(c) != vals.end())
		dir << c << "_" << std::hex << vals[c] << "/";
	    }
	    iBooker.setCurrentFolder(topFolderName + "/" + dir.str());
	    // TODO veery broken
	    if (e.second.th1->GetDimension() == 1) {
	      TAxis* ax = e.second.th1->GetXaxis();
	      e.second.me = iBooker.book1D(this->name, this->title, ax->GetNbins(), ax->GetXmin(), ax->GetXmax());
	    } else {
	      assert(!"NIY");
	    }
	    e.second.me->getTH1()->Add(e.second.th1);
	    //delete e.second.th1;
	    e.second.th1 = e.second.me->getTH1(); 
	  }
	}

	// Simple grouping. Drop colums, add histos if one is already present.
	if (step.type == SummationStep::GROUPBY) {
	  Table out;
	  for (auto& e : t) {
	    GeometryInterface::Values old_vals = e.first;
	    TH1 *th1 = e.second.th1;
	    GeometryInterface::Values new_vals;
	    for (auto col : step.columns) {
	      auto it = old_vals.find(col);
	      if (it != old_vals.end()) {
		new_vals.insert(*it);
	      }
	    }
	    AbstractHistogram& new_histo = out[new_vals];
	    if (!new_histo.th1) {
	      new_histo.th1 = (TH1*) th1->Clone();
	    } else {
	      new_histo.th1->Add(th1);
	    }
	  }
	  t.swap(out);
	}

	// TODO: more.

      }
    }
  }
}



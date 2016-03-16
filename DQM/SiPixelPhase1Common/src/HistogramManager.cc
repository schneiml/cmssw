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

  // TODO: this should go into GeometryInterface?
  edm::ESHandle<TrackerGeometry> tg;
  iSetup.get<TrackerDigiGeometryRecord>().get(tg);
  auto detector = tg->detIds();

  // TODO: We need 2 passes, one to count the elements for EXTEND-ranges, one for actual booking.
  for (unsigned int i = 0; i < specs.size(); i++) {
    auto& s = specs[i];
    auto& t = tables[i];
    for (DetId id : detector) {
      auto iq = GeometryInterface::InterestingQuantities{
	id, nullptr, 0, 0 // TODO: for fine geometry, iterate over all sane values here
      };
      auto dimensions = this->dimensions;
      std::stringstream name(this->name);
      std::stringstream dir("");
      std::stringstream title(this->title);
      std::stringstream xlabel(this->xlabel);
      std::stringstream ylabel(this->ylabel);
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
	title << ";" << xlabel << ";" << ylabel;
	histo.me = iBooker.book2D(name.str().c_str(), title.str().c_str(), 100, 0.0, 300.0, 100, 0.0, 300.0);
      } else {
	std::cout << "Booking " << dimensions << " dimensions not supported.\n";
      }
    }
  }
}


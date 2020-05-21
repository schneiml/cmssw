/*
 * \file RandomProjector.cc
 *
 * Harvesting module that computes a deterministic (hash-based) random projection
 * of *all* of DQM. This can then be used as a low-dimensional summary as an
 * input for Machine Learning methods.
 * \author Marcel Schneider 
 *
 */

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "DQMServices/Core/interface/DQMEDHarvester.h"

#include <cmath>
#include <random>
#include <memory>
#include <string>
#include <iostream>

class RandomProjector : public DQMEDHarvester {
public:
  /// Constructor
  RandomProjector(const edm::ParameterSet& ps);

  /// Destructor
  ~RandomProjector() override;

protected:
  void dqmEndLuminosityBlock(DQMStore::IBooker&,
                             DQMStore::IGetter&,
                             edm::LuminosityBlock const& lumiSeg,
                             edm::EventSetup const& c) override;
  void dqmEndJob(DQMStore::IBooker&, DQMStore::IGetter&) override;

private:
  void performProjection(DQMStore::IGetter& igetter, DQMStore::IBooker& ibooker, MonitorElementData::Scope scope);
  int ndimensions;
  std::string meprefix;
  std::string outputdir;
  bool writetostdout;
};

RandomProjector::RandomProjector(const edm::ParameterSet& ps) : DQMEDHarvester(ps) {
  ndimensions = ps.getUntrackedParameter<int>("ndimensions", 1000);
  meprefix = ps.getUntrackedParameter<std::string>("meprefix", "");
  outputdir = ps.getUntrackedParameter<std::string>("outputdir", "DQM/RandomProjections");
  writetostdout = ps.getUntrackedParameter<bool>("writetostdout", true);
}

RandomProjector::~RandomProjector() {}

void RandomProjector::dqmEndLuminosityBlock(DQMStore::IBooker& ibooker,
                                            DQMStore::IGetter& igetter,
                                            edm::LuminosityBlock const&,
                                            edm::EventSetup const&) {
  auto scope = DQMStore::IBooker::UseLumiScope(ibooker);
  performProjection(igetter, ibooker, MonitorElementData::Scope::LUMI);
}

void RandomProjector::dqmEndJob(DQMStore::IBooker& ibooker, DQMStore::IGetter& igetter) {
  //performProjection(igetter, ibooker, MonitorElementData::Scope::JOB);
}

void RandomProjector::performProjection(DQMStore::IGetter& igetter,
                                        DQMStore::IBooker& ibooker,
                                        MonitorElementData::Scope scope) {
  edm::LogInfo("RandomProjector") << "Performing Projection of '" << meprefix << "' to " << ndimensions
                                  << " dimensions";

  auto mes = igetter.getAllContents(meprefix);

  std::vector<double> outbuf(ndimensions, 0);

  int totbins = 0;

  for (auto me : mes) {
    if (me->getScope() != scope)
      continue;
    if (me->getEfficiencyFlag())
      continue;              // only real histograms here
    auto kind = me->kind();  // we are a bit picky with the types,  but better
    // be careful if we throw all of DQM together.
    if (!(kind == MonitorElement::Kind::TH1F || kind == MonitorElement::Kind::TH2F))
      continue;

    std::string name = me->getFullname();

    auto rand = std::minstd_rand();
    rand.seed(42);         // deterministic starting seed
    for (char c : name) {  // poor man's hash function
      rand();
      auto next = rand();
      rand.seed(next + c);
      rand();
    }
    // now rand() should be a nice random but reproducible sequence based on the ME name.

    auto weightdist = std::normal_distribution<double>(0.0, 1.0);

    if (kind == MonitorElement::Kind::TH1F) {
      auto nbins = me->getNbinsX();
      for (int i = 1; i <= nbins; i++) {
        double val = me->getBinContent(i);
        for (int currentdim = 0; currentdim < ndimensions; currentdim++) {
          double weight = weightdist(rand);
          outbuf[currentdim] += weight * val;
        }
        totbins++;
      }
    }
    if (kind == MonitorElement::Kind::TH2F) {
      auto nbinsx = me->getNbinsX();
      auto nbinsy = me->getNbinsY();
      for (int y = 1; y <= nbinsy; y++) {
        for (int x = 1; x <= nbinsx; x++) {
          double val = me->getBinContent(x, y);
          for (int currentdim = 0; currentdim < ndimensions; currentdim++) {
            double weight = weightdist(rand);
            outbuf[currentdim] += weight * val;
          }
          totbins++;
        }
      }
    }
  }

  ibooker.setCurrentFolder(outputdir);
  std::string outname = "randomprojection_" + meprefix;
  std::replace(outname.begin(), outname.end(), '/', '_');
  MonitorElement* out = ibooker.book1DD(outname, outname, ndimensions, 1, ndimensions);
  for (int i = 1; i <= ndimensions; i++) {
    out->setBinContent(i, outbuf[i - 1]);
  }

  edm::LogError("RandomProjector") << "Projected " << totbins << " bins from '" << meprefix << "' to "
                                   << out->getFullname() << "(" << ndimensions << " dimensions)";
  if (writetostdout) {
    edm::LuminosityBlockID runlumi = out->getRunLumi();
    std::cout << out->getFullname() << "," << runlumi.run() << "," << runlumi.luminosityBlock() << ",";
    for (int i = 1; i <= ndimensions; i++) {
      double val = out->getBinContent(i);
      std::cout << val << ",";
    }
    std::cout << "\n";
  }
}

DEFINE_FWK_MODULE(RandomProjector);

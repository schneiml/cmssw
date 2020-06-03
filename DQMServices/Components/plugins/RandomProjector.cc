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

// rather than using the std::minstd_rand for perf-ciritcal stuff, we re-implement the
// same algortihm:
// - Using a floating-point type
// - Without modulo
// - As a plain inline function operating on simple types.
// All of this to help the compiler with auto-vectorization.
typedef double rand_type;
static inline rand_type minstd_step(rand_type x) {
  // return (a * x + c) mod m
  rand_type acc = 0;
  rand_type m = 2147483647UL;
  // unrolled shift-add multiplication with modulo after every step, for constant 48271UL.
  // Maybe we could use a loop and rely on it being unrolled instead...
  acc += x;
  if (acc >= m)
    acc -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  acc += x;
  if (acc >= m)
    acc -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  acc += x;
  if (acc >= m)
    acc -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  acc += x;
  if (acc >= m)
    acc -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  acc += x;
  if (acc >= m)
    acc -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  acc += x;
  if (acc >= m)
    acc -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  acc += x;
  if (acc >= m)
    acc -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  acc += x;
  if (acc >= m)
    acc -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  acc += x;
  if (acc >= m)
    acc -= m;
  x = x * 2;
  if (x >= m)
    x -= m;
  acc += x;
  if (acc >= m)
    acc -= m;
  return acc;
}

// The vectorized main loop is in this function, to
// - be able to use __restrict__ pointers.
// - make the compiler debug output mor readable.
// - Help the compiler being less confused.
typedef double accu_type;
static void doAccumulate(rand_type* __restrict__ localrands,
                         accu_type val,
                         accu_type* __restrict__ outbuf,
                         int ndimensions) {
  for (int currentdim = 0; currentdim < ndimensions; currentdim++) {
    rand_type x = minstd_step(localrands[currentdim]);
    rand_type r = x;
    localrands[currentdim] = x;
    accu_type weight = 0.f;
    // the random number has range 1..0x7FFFFFFF, these values are 1/6 and 2/6 of that.
    if (r < 715827882)
      weight = 1.f;  // 1/6'th each, remaining 2/3's stay 0.
    if (r < 357913941)
      weight = -1.f;
    outbuf[currentdim] += weight * val;
  }
}

void RandomProjector::performProjection(DQMStore::IGetter& igetter,
                                        DQMStore::IBooker& ibooker,
                                        MonitorElementData::Scope scope) {
  edm::LogInfo("RandomProjector") << "Performing Projection of '" << meprefix << "' to " << ndimensions
                                  << " dimensions";

  auto mes = igetter.getAllContents(meprefix);

  std::vector<accu_type> outbuf_vec(ndimensions, 0);
  accu_type* outbuf = &outbuf_vec[0];
  // we will need this later, and should not re-allocate the buffer.
  auto localrands_vec = std::vector<rand_type>(ndimensions);

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

    // then, we branch off one random generator per dimension to remove a
    // loop-carried dependency below. That should be enough to get the auto-vectorizer
    // involved.
    // We define out own minstd variant with 64 bit state for vectorization (padding!)
    auto localrands = &localrands_vec[0];
    for (int currentdim = 0; currentdim < ndimensions; currentdim++) {
      // offset seed a bit, else they all produce the same (offset) sequence
      localrands[currentdim] = rand() + 42;
    }

    //auto weightdist = std::uniform_real_distribution<double>(-1, 1);
    //auto weightdist = std::normal_distribution<double>(0, 1);
    // Using Achlioptas' discrete trick: use sqrt(3) * {+1, 0, -1} as weights.
    // We can drop the sqrt(3) since we don't care about the linear scaling.
    // That leaves the distribution between the three, which should be 1/6, 4/6, 1/6.
    // Sixth's are hard as a power of two, but we can approximate using a lot of bits.

    float* buf = nullptr;
    int nbins = 0;
    if (kind == MonitorElement::Kind::TH1F) {
      // unsafe access, but what we do should be sort-of safe.
      buf = me->getTH1F()->GetArray();
      // this includes overflows etc.
      nbins = me->getTH1F()->GetSize();
    }
    if (kind == MonitorElement::Kind::TH2F) {
      buf = me->getTH2F()->GetArray();
      nbins = me->getTH2F()->GetSize();
    }
    if (buf != nullptr) {
      for (int i = 0; i < nbins; i++) {
        float val = buf[i];
        if (isnan(val) || isinf(val))
          val = 0;
        doAccumulate(localrands, val, outbuf, ndimensions);
      }
      totbins += nbins;
    }
  }

  ibooker.setCurrentFolder(outputdir);
  std::string outname = "randomprojection_" + meprefix;
  std::replace(outname.begin(), outname.end(), '/', '_');
  MonitorElement* out = ibooker.book1DD(outname, outname, ndimensions, 1, ndimensions);
  for (int i = 1; i <= ndimensions; i++) {
    out->setBinContent(i, outbuf[i - 1]);
  }

  edm::LogInfo("RandomProjector") << "Projected " << totbins << " bins from '" << meprefix << "' to "
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

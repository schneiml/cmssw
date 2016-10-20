// A small plugin that does not really look at data, but metadata:
// processing time spent and real time between events.
//
// This is not and cannot be thread-safe; it is only for online.
//
// Original Author: Marcel Schneider

#include "DQM/SiPixelPhase1Common/interface/SiPixelPhase1Base.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include <chrono>

class SiPixelPhase1OnlineTiming : public SiPixelPhase1Base {
  enum {
    PROCESSING_TIME,
    PROCESSING_TIME_LS,
    REAL_EVENT_RATE
  };

  typedef std::chrono::steady_clock::time_point time_point;

  public:
  explicit SiPixelPhase1OnlineTiming(const edm::ParameterSet& conf) 
           : SiPixelPhase1Base(conf) {};
  void analyze(const edm::Event& ev, const edm::EventSetup& es) {
    uint32_t lumi = ev.luminosityBlock();
    int32_t orbit = ev.orbitNumber();
    int32_t bx    = ev.bunchCrossing();
    auto    now   = std::chrono::steady_clock::now();
    
    if (lumi == last_lumi) {
      double dt = (orbit-last_orbit)*3600*25e-9 + (bx - last_bx) * 25e-9;
      double ct = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_ev).count() * 1e-9;
      histo[REAL_EVENT_RATE].fill(dt, DetId(0), &ev);
      histo[PROCESSING_TIME].fill(ct, DetId(0), &ev);
    } else {
      if (last_ls != time_point()) {
        double ct = std::chrono::duration_cast<std::chrono::nanoseconds>(now - last_ls).count() * 1e-9;
        histo[PROCESSING_TIME_LS].fill(ct, DetId(0), &ev);
      }
      last_ls = now; 
    }

    last_ev = now;
    last_lumi = lumi;
    last_orbit = orbit;
    last_bx = bx;
  }

  private:
  uint32_t last_lumi = 0;
  int32_t last_orbit = 0;
  int32_t last_bx = 0;
  time_point last_ev;
  time_point last_ls;
};

DEFINE_FWK_MODULE(SiPixelPhase1OnlineTiming);


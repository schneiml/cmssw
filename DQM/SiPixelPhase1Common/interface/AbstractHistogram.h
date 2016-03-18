#ifndef SiPixel_AbstractHistogram_h
#define SiPixel_AbstractHistogram_h
// -*- C++ -*-
//
// Package:    SiPixelPhase1Common
// Class:      AbstractHistogram
//
// This is a spaceholder for a histogram in 0, 1, or 2 Dimensions. May or may 
// not be backed by a TH1 or similar. May not be there at all and created on 
// demand. Mainly designed as a value in std::map.
//
// Original Author:  Marcel Schneider
//

#include "DQMServices/Core/interface/MonitorElement.h"
#include <vector>
#include <utility>
#include <cassert>

struct AbstractHistogram {

  void fill(double x, double y) {
    if (me) {
      me->Fill(x, y);
      return;
    } else if (th1) {
      th1->Fill(x, y);
    } else {
      assert(!"New Th1 needed but parameters missing");
      //th1 = new TH2D(..);
    } 
  }; 
  
  void fill(double x) {
    if (me) {
      me->Fill(x);
      return;
    } else if (th1) {
      th1->Fill(x);
    } else {
      assert(!"New Th1 needed but parameters missing");
      //th1 = new TH1D(..);
    }
  };
  
  void fill() {
    assert(!me && !th1);
    value++;
  };
  
  double value = 0;
  MonitorElement* me = nullptr;
  TH1* th1 = nullptr;

  ~AbstractHistogram() {
    // if both are set the ME should own the TH1
    if (th1 && !me) delete th1;
  };

};


#endif

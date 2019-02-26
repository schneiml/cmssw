#ifndef DQMSERVICES_CORE_DQMSTORE_H
#define DQMSERVICES_CORE_DQMSTORE_H

#include "DQMServices/Core/interface/MonitorElement.h"

#include <limits>

#define INF (std::numeric_limits<double>::infinity())
//#define INF (1.0/0.0)

class DQMStore {

public:
  class IBooker {
  public:
    void setCurrentFolder(std::string folder) {};
    MonitorElement* bookInt(std::string name, std::string title = "") { return nullptr; };
    MonitorElement* bookString(std::string name, std::string title = "") { return nullptr; };

   // TODO: replicating all the variations of arguments here requires more work
   // This section catches many ambiguous numeric variations, likely incorrectly.
    MonitorElement* book1D(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0) { return nullptr; };
    MonitorElement* book2D(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
        int ynbins = 30, double ylow = 0.0, double yhigh = 1.0) { return nullptr; };
    // ynbins is useless here, should be dropped.
    MonitorElement* bookProfile(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
        int ynbins = 0, double ylow = -INF, double yhigh = +INF) { return nullptr; };
    MonitorElement* bookProfile2D(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
        int ynbins = 30, double ylow = 0.0, double yhigh = 1.0,
        double zlow = -INF, double zhigh = +INF) { return nullptr; };

    // TProfile takes options. Maybe get rid of that?
    DQM_DEPRECATED MonitorElement* bookProfile(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
        double ylow = -INF, double yhigh = +INF, std::string opts = "") { return nullptr; };
    // a version taking bare buffers?
    DQM_DEPRECATED MonitorElement* book1D(std::string name, std::string title, 
        int xnbins, const float* buf) { return nullptr; };
    DQM_DEPRECATED MonitorElement* bookProfile(std::string name, std::string title, 
        int xnbins, const double* buf,
        int ynbbins, double ylow, double yhigh) { return nullptr; };

    // A version of each taking a existing ROOT object.
    MonitorElement* book1D(std::string name, TH1* object) { return nullptr; };
    MonitorElement* book2D(std::string name, TH2* object) { return nullptr; };
    MonitorElement* bookProfile(std::string name, TProfile* object) { return nullptr; };
    MonitorElement* bookProfile2D(std::string name, TProfile2D* object) { return nullptr; };
  };

  class IGetter {
  public:
    MonitorElement* get(std::string name) { return nullptr; };

  };

  DQM_DEPRECATED void save(std::string filename) {};
  DQM_DEPRECATED void setVerbose(int level) {};
  DQM_DEPRECATED void showDirStructure() {};
  DQM_DEPRECATED void setCurrentFolder(std::string folder) {};
  DQM_DEPRECATED MonitorElement* get(std::string name) { return nullptr; };


  // APIs to be removed
  template <typename iFunc>
  DQM_DEPRECATED void meBookerGetter(iFunc f)
  {
  }

 

};

#undef INF

#endif

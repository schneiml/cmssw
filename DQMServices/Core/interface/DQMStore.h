#ifndef DQMSERVICES_CORE_DQMSTORE_H
#define DQMSERVICES_CORE_DQMSTORE_H

#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/ConcurrentMonitorElement.h"

#include <limits>

#define INF (std::numeric_limits<double>::infinity())
//#define INF (1.0/0.0)

class DQMStore {

public:
  class IBooker {
  public:
    void setCurrentFolder(std::string folder) {};
    DQM_DEPRECATED void cd(std::string folder = "") {};
    DQM_DEPRECATED void goUp() {};
    DQM_DEPRECATED std::string pwd() { return ""; };

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
        int ynbins = 0, double ylow = -INF, double yhigh = +INF, std::string opts = "") { return nullptr; };
    MonitorElement* bookProfile2D(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
        int ynbins = 30, double ylow = 0.0, double yhigh = 1.0,
        double zlow = -INF, double zhigh = +INF) { return nullptr; };

    // TProfile takes options. 
    MonitorElement* bookProfile(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
        double ylow = -INF, double yhigh = +INF, std::string opts = "") { return nullptr; };
    // a version taking bare buffers?
    DQM_DEPRECATED MonitorElement* book1D(std::string name, std::string title, 
        int xnbins, const float* buf) { return nullptr; };
    DQM_DEPRECATED MonitorElement* bookProfile(std::string name, std::string title, 
        int xnbins, const double* buf,
        int ynbbins, double ylow, double yhigh) { return nullptr; };
    DQM_DEPRECATED MonitorElement* bookProfile(std::string name, std::string title = "", 
        int xnbins = 30, double* buf = nullptr, // ???
        double ylow = -INF, double yhigh = +INF, std::string opts = "") { return nullptr; };

    // A version of each taking a existing ROOT object.
    MonitorElement* book1D(std::string name, TH1* object) { return nullptr; };
    MonitorElement* book2D(std::string name, TH2* object) { return nullptr; };
    MonitorElement* book3D(std::string name, TH3* object) { return nullptr; };
    MonitorElement* bookProfile(std::string name, TProfile* object) { return nullptr; };
    MonitorElement* bookProfile2D(std::string name, TProfile2D* object) { return nullptr; };

    DQM_DEPRECATED void tag(MonitorElement* me, unsigned int tag);
    DQM_DEPRECATED void tagContents(std::string, unsigned int tag);
  };

  class ConcurrentBooker {
  public:
    void setCurrentFolder(std::string folder) {};
    DQM_DEPRECATED void cd() {};

    ConcurrentMonitorElement bookInt(std::string name, std::string title = "") { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement bookString(std::string name, std::string title = "") { return ConcurrentMonitorElement(); };

   // TODO: replicating all the variations of arguments here requires more work
   // This section catches many ambiguous numeric variations, likely incorrectly.
    ConcurrentMonitorElement book1D(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2D(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
        int ynbins = 30, double ylow = 0.0, double yhigh = 1.0) { return ConcurrentMonitorElement(); };
    // ynbins is useless here, should be dropped.
    ConcurrentMonitorElement bookProfile(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
        int ynbins = 0, double ylow = -INF, double yhigh = +INF, std::string opts = "") { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement bookProfile2D(std::string name, std::string title = "", 
        int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
        int ynbins = 30, double ylow = 0.0, double yhigh = 1.0,
        int znbins = 0,  double zlow = -INF, double zhigh = +INF, std::string opts = "") { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book1D(std::string name, TH1* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2D(std::string name, TH2* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book3D(std::string name, TH3* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement bookProfile(std::string name, TProfile* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement bookProfile2D(std::string name, TProfile2D* object) { return ConcurrentMonitorElement(); };
  };

  class IGetter {
  public:
    MonitorElement* get(std::string name) { return nullptr; };
    DQM_DEPRECATED void removeElement(std::string name) {};
    std::vector<MonitorElement*> getAllContents(std::string path = "") { return {}; };
    DQM_DEPRECATED std::vector<MonitorElement*> getContents(std::string path = "", unsigned int tag = 0) { return {}; };
    DQM_DEPRECATED std::vector<std::string> getMEs() { return {}; };

    DQM_DEPRECATED bool dirExists(std::string name) { return false; };
    DQM_DEPRECATED std::vector<std::string> getSubdirs() { return {}; };

    DQM_DEPRECATED void setCurrentFolder(std::string folder) {};
    DQM_DEPRECATED void cd(std::string folder = "") {};

  };

  DQM_DEPRECATED void save(std::string filename) {};
  DQM_DEPRECATED void open(std::string filename, bool) {};
  DQM_DEPRECATED void setVerbose(int level) {};
  DQM_DEPRECATED void showDirStructure() {};
  DQM_DEPRECATED void setCurrentFolder(std::string folder) {};
  DQM_DEPRECATED MonitorElement* get(std::string name) { return nullptr; };


  // legacy booking interface
  DQM_DEPRECATED void cd(std::string folder = "") {};
  DQM_DEPRECATED std::string pwd() { return ""; };
  DQM_DEPRECATED MonitorElement* book1D(std::string name, TH1* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book1S(std::string name, TH1* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2D(std::string name, TH2* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2S(std::string name, TH2* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book3D(std::string name, TH3* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* bookProfile(std::string name, TProfile* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* bookProfile2D(std::string name, TProfile2D* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book1D(std::string name, std::string title = "", 
    int xnbins = 30, double xlow = 0.0, double xhigh = 1.0) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2D(std::string name, std::string title = "", 
    int xnbins = 30, double xlow = 0.0, double xhigh = 1.0,
    int ynbins = 30, double ylow = 0.0, double yhigh = 1.0) { return nullptr; };

  DQM_DEPRECATED std::vector<std::string> getSubdirs() { return {}; };
  DQM_DEPRECATED std::vector<std::string> getMEs() { return {}; };
  DQM_DEPRECATED std::vector<MonitorElement*> getAllContents(std::string path = "") { return {}; };
  DQM_DEPRECATED void rmdir(std::string) {};

  // APIs to be removed
  template <typename iFunc>
  DQM_DEPRECATED void meBookerGetter(iFunc f)
  {
  }

 

};

#undef INF

#endif

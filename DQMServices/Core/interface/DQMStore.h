#ifndef DQMSERVICES_CORE_DQMSTORE_H
#define DQMSERVICES_CORE_DQMSTORE_H

#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMDefinitions.h"
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
    MonitorElement* bookFloat(std::string name, std::string title = "") { return nullptr; };
    MonitorElement* bookString(std::string name, std::string title = "") { return nullptr; };

#define NAMETITLE TString const& name, TString const& title
#define XBINRANGE int xnbins, double xlow, double xhigh
#define YBINRANGE int ynbins, double ylow, double yhigh
#define ZBINRANGE int znbins, double zlow, double zhigh
#define XBINVALUES int xnbins, float* xbinvalues
#define YBINVALUES int ynbins, float* ybinvalues
#define YRANGE double ylow, double yhigh
#define ZRANGE double zlow, double zhigh
    MonitorElement* book1D(NAMETITLE, XBINRANGE) { return nullptr; };
    MonitorElement* book1D(NAMETITLE, XBINVALUES) { return nullptr; };
    MonitorElement* book2D(NAMETITLE, XBINRANGE, YBINRANGE) { return nullptr; };
    MonitorElement* book2D(NAMETITLE, XBINVALUES, YBINVALUES) { return nullptr; };
    MonitorElement* book3D(NAMETITLE, XBINRANGE, YBINRANGE, ZBINRANGE) { return nullptr; };
    MonitorElement* book1DD(NAMETITLE, XBINRANGE) { return nullptr; };
    MonitorElement* book1DD(NAMETITLE, XBINVALUES) { return nullptr; };
    MonitorElement* book2DD(NAMETITLE, XBINRANGE, YBINRANGE) { return nullptr; };
    MonitorElement* book2DD(NAMETITLE, XBINVALUES, YBINVALUES) { return nullptr; };
    MonitorElement* book1S(NAMETITLE, XBINRANGE) { return nullptr; };
    MonitorElement* book1S(NAMETITLE, XBINVALUES) { return nullptr; };
    MonitorElement* book2S(NAMETITLE, XBINRANGE, YBINRANGE) { return nullptr; };
    MonitorElement* book2S(NAMETITLE, XBINVALUES, YBINVALUES) { return nullptr; };

    MonitorElement* bookProfile(NAMETITLE, XBINRANGE, YRANGE, const char* opts = "") { return nullptr; };
    MonitorElement* bookProfile(NAMETITLE, XBINRANGE, YBINRANGE, const char* opts = "") { return nullptr; };
    MonitorElement* bookProfile2D(NAMETITLE, XBINRANGE, YBINRANGE, ZRANGE, const char* opts = "") { return nullptr; };
    MonitorElement* bookProfile2D(NAMETITLE, XBINRANGE, YBINRANGE, ZBINRANGE, const char* opts = "") { return nullptr; };

    MonitorElement* book1D(std::string name, TH1* object) { return nullptr; };
    MonitorElement* book2D(std::string name, TH2* object) { return nullptr; };
    MonitorElement* book3D(std::string name, TH3* object) { return nullptr; };
    MonitorElement* book1DD(std::string name, TH1* object) { return nullptr; };
    MonitorElement* book2DD(std::string name, TH2* object) { return nullptr; };
    MonitorElement* book1S(std::string name, TH1* object) { return nullptr; };
    MonitorElement* book2S(std::string name, TH2* object) { return nullptr; };
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
    ConcurrentMonitorElement bookFloat(std::string name, std::string title = "") { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement bookString(std::string name, std::string title = "") { return ConcurrentMonitorElement(); };

    ConcurrentMonitorElement book1D(NAMETITLE, XBINRANGE) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book1D(NAMETITLE, XBINVALUES) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2D(NAMETITLE, XBINRANGE, YBINRANGE) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2D(NAMETITLE, XBINVALUES, YBINVALUES) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book3D(NAMETITLE, XBINRANGE, YBINRANGE, ZBINRANGE) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book1DD(NAMETITLE, XBINRANGE) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book1DD(NAMETITLE, XBINVALUES) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2DD(NAMETITLE, XBINRANGE, YBINRANGE) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2DD(NAMETITLE, XBINVALUES, YBINVALUES) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book1S(NAMETITLE, XBINRANGE) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book1S(NAMETITLE, XBINVALUES) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2S(NAMETITLE, XBINRANGE, YBINRANGE) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2S(NAMETITLE, XBINVALUES, YBINVALUES) { return ConcurrentMonitorElement(); };

    ConcurrentMonitorElement bookProfile(NAMETITLE, XBINRANGE, YRANGE, const char* opts = "") { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement bookProfile(NAMETITLE, XBINRANGE, YBINRANGE, const char* opts = "") { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement bookProfile2D(NAMETITLE, XBINRANGE, YBINRANGE, ZRANGE, const char* opts = "") { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement bookProfile2D(NAMETITLE, XBINRANGE, YBINRANGE, ZBINRANGE, const char* opts = "") { return ConcurrentMonitorElement(); };

    ConcurrentMonitorElement book1D(std::string name, TH1* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2D(std::string name, TH2* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book3D(std::string name, TH3* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book1DD(std::string name, TH1* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2DD(std::string name, TH2* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book1S(std::string name, TH1* object) { return ConcurrentMonitorElement(); };
    ConcurrentMonitorElement book2S(std::string name, TH2* object) { return ConcurrentMonitorElement(); };
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

  enum OpenRunDirs {
    KeepRunDirs,
    StripRunDirs
  };
  DQM_DEPRECATED void save(std::string filename) {};
  DQM_DEPRECATED void open(std::string filename, bool something = false, std::string a = "", std::string b = "", OpenRunDirs mode = KeepRunDirs) {}; // last args for reference?
  DQM_DEPRECATED void setVerbose(int level) {};
  DQM_DEPRECATED void showDirStructure() {};
  DQM_DEPRECATED MonitorElement* get(std::string name) { return nullptr; };


  // legacy booking interface
  DQM_DEPRECATED void setCurrentFolder(std::string folder) {};
  DQM_DEPRECATED void cd(std::string folder = "") {};
  DQM_DEPRECATED void goUp() {};
  DQM_DEPRECATED std::string pwd() { return ""; };

  DQM_DEPRECATED MonitorElement* bookInt(std::string name, std::string title = "") { return nullptr; };
  DQM_DEPRECATED MonitorElement* bookFloat(std::string name, std::string title = "") { return nullptr; };
  DQM_DEPRECATED MonitorElement* bookString(std::string name, std::string title = "") { return nullptr; };

  DQM_DEPRECATED MonitorElement* book1D(NAMETITLE, XBINRANGE) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book1D(NAMETITLE, XBINVALUES) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2D(NAMETITLE, XBINRANGE, YBINRANGE) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2D(NAMETITLE, XBINVALUES, YBINVALUES) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book3D(NAMETITLE, XBINRANGE, YBINRANGE, ZBINRANGE) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book1DD(NAMETITLE, XBINRANGE) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book1DD(NAMETITLE, XBINVALUES) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2DD(NAMETITLE, XBINRANGE, YBINRANGE) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2DD(NAMETITLE, XBINVALUES, YBINVALUES) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book1S(NAMETITLE, XBINRANGE) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book1S(NAMETITLE, XBINVALUES) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2S(NAMETITLE, XBINRANGE, YBINRANGE) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2S(NAMETITLE, XBINVALUES, YBINVALUES) { return nullptr; };

  DQM_DEPRECATED MonitorElement* bookProfile(NAMETITLE, XBINRANGE, YRANGE, const char* opts = "") { return nullptr; };
  DQM_DEPRECATED MonitorElement* bookProfile(NAMETITLE, XBINRANGE, YBINRANGE, const char* opts = "") { return nullptr; };
  DQM_DEPRECATED MonitorElement* bookProfile2D(NAMETITLE, XBINRANGE, YBINRANGE, ZRANGE, const char* opts = "") { return nullptr; };
  DQM_DEPRECATED MonitorElement* bookProfile2D(NAMETITLE, XBINRANGE, YBINRANGE, ZBINRANGE, const char* opts = "") { return nullptr; };

  DQM_DEPRECATED MonitorElement* book1D(std::string name, TH1* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2D(std::string name, TH2* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book3D(std::string name, TH3* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book1DD(std::string name, TH1* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2DD(std::string name, TH2* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book1S(std::string name, TH1* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* book2S(std::string name, TH2* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* bookProfile(std::string name, TProfile* object) { return nullptr; };
  DQM_DEPRECATED MonitorElement* bookProfile2D(std::string name, TProfile2D* object) { return nullptr; };

  DQM_DEPRECATED void tag(MonitorElement* me, unsigned int tag);
  DQM_DEPRECATED void tagContents(std::string, unsigned int tag);

  // legacy getter interface
  DQM_DEPRECATED bool dirExists(std::string name) { return false; };
  DQM_DEPRECATED std::vector<std::string> getSubdirs() { return {}; };
  DQM_DEPRECATED std::vector<std::string> getMEs() { return {}; };
  DQM_DEPRECATED std::vector<MonitorElement*> getAllContents(std::string path = "") { return {}; };
  DQM_DEPRECATED void rmdir(std::string) {};
  DQM_DEPRECATED void removeElement(std::string name) {};

  // APIs to be removed
  template <typename iFunc>
  DQM_DEPRECATED void meBookerGetter(iFunc f)
  {
  }
  enum SaveReferenceTag {
    SaveWithoutReference,
    SaveWithReference,
    SaveWithReferenceForQTest
  };

  DQM_DEPRECATED bool load(std::string filename, OpenRunDirs mode, bool something = false){ return false; };
  DQM_DEPRECATED void tag(MonitorElement* me, int tag) {};


 

};

#undef INF

#endif

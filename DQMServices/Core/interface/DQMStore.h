#ifndef DQMSERVICES_CORE_DQMSTORE_H
#define DQMSERVICES_CORE_DQMSTORE_H

#include "DQMServices/Core/interface/MonitorElement.h"

class DQMStore {

public:
  class IBooker {
  public:
    void setCurrentFolder(std::string folder) {};
    MonitorElement* bookInt(std::string name, std::string title = "") { return nullptr; };
    MonitorElement* bookString(std::string name, std::string title = "") { return nullptr; };
    MonitorElement* book1D(std::string name, std::string title = "", int nbins = 30, double xlow = 0.0, double xhigh = 1.0) { return nullptr; };



  };

  class IGetter {

  };

  DQM_DEPRECATED void save(std::string filename) {};

};

#endif

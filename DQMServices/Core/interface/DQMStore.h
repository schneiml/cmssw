#ifndef DQMSERVICES_CORE_DQMSTORE_H
#define DQMSERVICES_CORE_DQMSTORE_H

class DQMStore {

public:
  class IBooker {
  public:
    void setCurrentFolder(std::string folder) {};
    MonitorElement* bookInt(std::string name, std::string title = "") { return nullptr; };
    MonitorElement* bookString(std::string name, std::string title = "") { return nullptr; };

  };

  class IGetter {

  };

};

#endif

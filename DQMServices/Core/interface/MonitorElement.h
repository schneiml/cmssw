#ifndef DQMSERVICES_CORE_MONITORELEMENT_H
#define DQMSERVICES_CORE_MONITORELEMENT_H

# if __GNUC__ && ! defined DQM_DEPRECATED
#  define DQM_DEPRECATED __attribute__((deprecated))
# endif


# include "TF1.h"
# include "TH1F.h"
# include "TH1S.h"
# include "TH1D.h"
# include "TH2F.h"
# include "TH2S.h"
# include "TH2D.h"
# include "TH3F.h"
# include "TProfile.h"
# include "TProfile2D.h"


class MonitorElement {
public:

  // Fill() calls
  void Fill(std::string value) {};

  void Fill(double x) {};
  void Fill(double x, double y) {};
  void Fill(double x, double y, double z) {};
  // The APIs taking weights are ambiguous, we should rename them.
  DQM_DEPRECATED void Fill(double x, double y, double z, double w) {};

  // Other setters
  void setAxisTitle(std::string title, int axis = 0) {};
  DQM_DEPRECATED void setXTitle(std::string title) {};
  DQM_DEPRECATED void setYTitle(std::string title) {};
  DQM_DEPRECATED void setAxisRange(double, double, int) {};

  // Maybe deprecated?
  DQM_DEPRECATED void setBinLabel(int binindex, std::string label, int axis = 0) {};

  DQM_DEPRECATED void Reset() {};

  DQM_DEPRECATED TObject* getRootObject() { return nullptr; };
  DQM_DEPRECATED TH1* getTH1() { return nullptr; };
  DQM_DEPRECATED TH1F* getTH1F() { return nullptr; };
  DQM_DEPRECATED TH1D* getTH1D() { return nullptr; };
  DQM_DEPRECATED TH2F* getTH2F() { return nullptr; };
  DQM_DEPRECATED TH2D* getTH2D() { return nullptr; };
  DQM_DEPRECATED TH2S* getTH2S() { return nullptr; };
  DQM_DEPRECATED TProfile* getTProfile() { return nullptr; };
  DQM_DEPRECATED TProfile2D* getTProfile2D() { return nullptr; };

  int getIntValue() { return 0; };
  float getFloatValue() { return 0; };


  DQM_DEPRECATED std::string getTitle() const { return ""; };
  DQM_DEPRECATED void setTitle(std::string) const {};
  DQM_DEPRECATED std::string getFullname() { return ""; };
  DQM_DEPRECATED std::string getPathname() { return ""; };
  DQM_DEPRECATED std::string getName() { return ""; };

  // non-histogram operations -- can we force people to use TProfile instead?
  DQM_DEPRECATED double getEntries() { return 0; };
  DQM_DEPRECATED void setEntries(double) {};
  DQM_DEPRECATED void setBinEntries(int, double) {};
  DQM_DEPRECATED double getBinEntries(int) { return 0; };
  DQM_DEPRECATED void setBinContent(int, double) {};
  DQM_DEPRECATED void setBinContent(int, int, double) {};
  DQM_DEPRECATED void setBinError(int, double) {};
  DQM_DEPRECATED double getBinError(int) { return 0; };
  DQM_DEPRECATED void setBinError(int, int, double) {};
  DQM_DEPRECATED double getBinError(int, int) { return 0; };
  DQM_DEPRECATED int getNbinsX() { return 0; };
  DQM_DEPRECATED int getNbinsY() { return 0; };
  DQM_DEPRECATED double getBinContent(int bin) { return 0; };
  DQM_DEPRECATED double getBinContent(int, int) { return 0; };
  DQM_DEPRECATED double getMean(int bin = 0) { return 0; };
  DQM_DEPRECATED double getMeanError(int bin = 0) { return 0; };
  DQM_DEPRECATED double getRMS(int bin = 0) { return 0; };

  DQM_DEPRECATED void setEfficiencyFlag(bool set = true) {};
  DQM_DEPRECATED void setLumiFlag(bool set = true) {};

  // QTest API
  //QReport getQReport(std::string qtestname);
  //std::vector<QReport> getQReports();
  bool hasError() { return false; }
  bool hasWarning() { return false; }
  bool hasOtherReport() { return false; }

  // almost unused things (BeamMonitor uses many)
  DQM_DEPRECATED void setResetMe(bool) {};
  DQM_DEPRECATED std::string valueString() { return ""; }
  DQM_DEPRECATED TObject* getRefRootObject() { return nullptr; }
  DQM_DEPRECATED TH1* getRefTH1() { return nullptr; }

  DQM_DEPRECATED void update() {};
  DQM_DEPRECATED void doFill(int64_t) {}; // ??
  DQM_DEPRECATED void ShiftFillLast(double, double, int) {};
  //std::vector<QReport> getQWarnings();
  //std::vector<QReport> getQErrors();
  DQM_DEPRECATED double getRMSError(int) { return 0; };
  DQM_DEPRECATED std::string getAxisTitle(int) { return ""; };
  DQM_DEPRECATED void setAxisTimeDisplay(int, int) {};
  DQM_DEPRECATED void setAxisTimeFormat(std::string, int) {};
  DQM_DEPRECATED void softReset() {};
  // DQM_DEPRECATED getTags();


  // Maybe this should be deprecated as well, but there are a lot of users and it is not too bad to have.
  enum Kind {
    DQM_KIND_INVALID,
    DQM_KIND_INT,
    DQM_KIND_REAL,
    DQM_KIND_STRING,
    DQM_KIND_TH1F,
    DQM_KIND_TH1S,
    DQM_KIND_TH1D,
    DQM_KIND_TH2F,
    DQM_KIND_TH2S,
    DQM_KIND_TH2D,
    DQM_KIND_TH3F,
    DQM_KIND_TPROFILE,
    DQM_KIND_TPROFILE2D
  };

  MonitorElement::Kind kind() { return DQM_KIND_INVALID; }

};

#endif

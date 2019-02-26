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

  // Maybe deprecated?
  DQM_DEPRECATED void setBinLabel(int binindex, std::string label) {};

  DQM_DEPRECATED void Reset() {};

  DQM_DEPRECATED TObject* getRootObject() { return nullptr; };
  DQM_DEPRECATED TH1* getTH1() { return nullptr; };
  DQM_DEPRECATED TH1F* getTH1F() { return nullptr; };
  DQM_DEPRECATED TH2F* getTH2F() { return nullptr; };
  DQM_DEPRECATED TProfile2D* getTProfile2D() { return nullptr; };

  DQM_DEPRECATED std::string getTitle() const { return ""; };

  // non-histogram operations -- can we force people to use TProfile instead?
  DQM_DEPRECATED double getEntries() { return 0; };
  DQM_DEPRECATED void setEntries(double) {};
  DQM_DEPRECATED void setEfficiencyFlag(bool set = true) {};
  DQM_DEPRECATED void setBinContent(int, double) {};
  DQM_DEPRECATED void setBinError(int, double) {};


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

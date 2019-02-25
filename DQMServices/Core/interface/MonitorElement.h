#ifndef DQMSERVICES_CORE_MONITORELEMENT_H
#define DQMSERVICES_CORE_MONITORELEMENT_H

# if __GNUC__ && ! defined DQM_DEPRECATED
#  define DQM_DEPRECATED __attribute__((deprecated))
# endif


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

  void Fill(double x) {};
  void Fill(double x, double y) {};
  void Fill(double x, double y, double z) {};
  void Fill(std::string value) {};

  DQM_DEPRECATED void Reset();

};

#endif

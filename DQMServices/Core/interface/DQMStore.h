#ifndef DQMServices_Core_DQMStore_h
#define DQMServices_Core_DQMStore_h

#if __GNUC__ && ! defined DQM_DEPRECATED
#define DQM_DEPRECATED __attribute__((deprecated))
#endif

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <iosfwd>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <set>
#include <string>
#include <thread>
#include <vector>
#include <memory>
#include <cxxabi.h>
#include <execinfo.h>

#include <classlib/utils/Regexp.h>


struct DQMChannel
{
  int binx;      //< bin # in x-axis (or bin # for 1D histogram)
  int biny;      //< bin # in y-axis (for 2D or 3D histograms)
  int binz;      //< bin # in z-axis (for 3D histograms)
  float content; //< bin content
  float RMS;     //< RMS of bin content

  int getBin()        { return getBinX(); }
  int getBinX()       { return binx; }
  int getBinY()       { return biny; }
/* unused */
/* almost unused */   int getBinZ()       { return binz; }
  float getContents() { return content; }
/* unused */
/* almost unused */   float getRMS()      { return RMS; }

  DQMChannel(int bx, int by, int bz, float data, float rms)
    {
      binx = bx;
      biny = by;
      binz = bz;
      content = data;
      RMS = rms;
    }

/* unused */
  DQMChannel()
    {
      binx = 0;
      biny = 0;
      binz = 0;
      content = 0;
      RMS = 0;
    }
};

namespace dqm
{
  namespace me_util
  {
    using Channel = DQMChannel;
  }
}

# include "DQMServices/Core/interface/DQMNet.h"
class QCriterion;

/** Class for reporting results of quality tests for Monitoring Elements */
class QReport
{
public:
  /// get test status (see Core/interface/QTestStatus.h)
  int getStatus() const
    { return qvalue_->code; }

  /// get test result i.e. prob value
  float getQTresult() const
    { return qvalue_->qtresult; }

  /// get message attached to test
  const std::string &getMessage() const
    { return qvalue_->message; }

  /// get name of quality test
/* unused */
  const std::string &getQRName() const
    { return qvalue_->qtname; }

  /// get vector of channels that failed test
  /// (not relevant for all quality tests!)
  const std::vector<DQMChannel> &getBadChannels() const
    { return badChannels_; }

  /// get QCriterion
/* unused */
/* almost unused */   const QCriterion *getQCriterion() const
    { return qcriterion_; }

private:
  friend class QCriterion;
  friend class MonitorElement;  // for running the quality test
  friend class DQMStore;        // for setting QReport parameters after receiving report

  QReport(DQMNet::QValue *value, QCriterion *qc)
    : qvalue_ (value),
      qcriterion_ (qc)
    {}

  DQMNet::QValue          *qvalue_;     //< Pointer to the actual data.
  QCriterion              *qcriterion_; //< Pointer to QCriterion algorithm.
  std::vector<DQMChannel> badChannels_; //< Bad channels from QCriterion.
}; 



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
# include "TObjString.h"
# include "TAxis.h"
# include <sys/time.h>
# include <string>
# include <set>
# include <map>
# include <sstream>
# include <iomanip>
# include <cassert>
# include <cstdint>

# ifndef DQM_ROOT_METHODS
#  define DQM_ROOT_METHODS 1
# endif

class QCriterion;

// tag for a special constructor, see below
struct MonitorElementNoCloneTag {};

/** The base class for all MonitorElements (ME) */
class MonitorElement
{
  friend class DQMStore;
  friend class DQMService;
public:
  struct Scalar
  {
    int64_t             num;
    double              real;
    std::string         str;
  };

  enum Kind
  {
    DQM_KIND_INVALID    = DQMNet::DQM_PROP_TYPE_INVALID,
    DQM_KIND_INT        = DQMNet::DQM_PROP_TYPE_INT,
    DQM_KIND_REAL       = DQMNet::DQM_PROP_TYPE_REAL,
    DQM_KIND_STRING     = DQMNet::DQM_PROP_TYPE_STRING,
    DQM_KIND_TH1F       = DQMNet::DQM_PROP_TYPE_TH1F,
    DQM_KIND_TH1S       = DQMNet::DQM_PROP_TYPE_TH1S,
    DQM_KIND_TH1D       = DQMNet::DQM_PROP_TYPE_TH1D,
    DQM_KIND_TH2F       = DQMNet::DQM_PROP_TYPE_TH2F,
    DQM_KIND_TH2S       = DQMNet::DQM_PROP_TYPE_TH2S,
    DQM_KIND_TH2D       = DQMNet::DQM_PROP_TYPE_TH2D,
    DQM_KIND_TH3F       = DQMNet::DQM_PROP_TYPE_TH3F,
    DQM_KIND_TPROFILE   = DQMNet::DQM_PROP_TYPE_TPROF,
    DQM_KIND_TPROFILE2D = DQMNet::DQM_PROP_TYPE_TPROF2D
  };

private:
  DQMNet::CoreObject    data_;       //< Core object information.
  Scalar                scalar_;     //< Current scalar value.
  TH1                   *object_;    //< Current ROOT object value.
  TH1                   *reference_; //< Current ROOT reference object.
  TH1                   *refvalue_;  //< Soft reference if any.
  std::vector<QReport>  qreports_;   //< QReports associated to this object.

  MonitorElement *initialise(Kind kind);
/* almost unused */   MonitorElement *initialise(Kind kind, TH1 *rootobj);
/* almost unused */   MonitorElement *initialise(Kind kind, const std::string &value);
/* unused */
/* almost unused */   void globalize() {
    data_.moduleId = 0;
  }
/* almost unused */   void setLumi(uint32_t ls) {
    data_.lumi = ls;
  }

public:
  MonitorElement();
  MonitorElement(const std::string *path, const std::string &name);
  MonitorElement(const std::string *path, const std::string &name, uint32_t run, uint32_t moduleId);
  MonitorElement(const MonitorElement &, MonitorElementNoCloneTag);
  MonitorElement(const MonitorElement &);
  MonitorElement(MonitorElement &&);
  MonitorElement &operator=(const MonitorElement &) = delete;
  MonitorElement &operator=(MonitorElement &&) = delete;
  ~MonitorElement();

  /// Compare monitor elements, for ordering in sets.
/* almost unused */   bool operator<(const MonitorElement &x) const
    {
      return DQMNet::setOrder(data_, x.data_);
    }

  /// Check the consistency of the axis labels
  static bool CheckBinLabels(const TAxis* a1, const TAxis * a2);

  /// Get the type of the monitor element.
  Kind kind() const
    { return Kind(data_.flags & DQMNet::DQM_PROP_TYPE_MASK); }

  /// Get the object flags.
/* unused */
/* almost unused */   uint32_t flags() const
    { return data_.flags; }

  /// get name of ME
  const std::string &getName() const
    { return data_.objname; }

  /// get pathname of parent folder
/* unused */
  const std::string &getPathname() const
    { return *data_.dirname; }

  /// get full name of ME including Pathname
  const std::string getFullname() const
    {
      std::string path;
      path.reserve(data_.dirname->size() + data_.objname.size() + 2);
      path += *data_.dirname;
      if (! data_.dirname->empty())
        path += '/';
      path += data_.objname;
      return path;
    }

  /// true if ME was updated in last monitoring cycle
/* unused */
  bool wasUpdated() const
    { return data_.flags & DQMNet::DQM_PROP_NEW; }

  /// Mark the object updated.
/* unused */
  void update()
    { data_.flags |= DQMNet::DQM_PROP_NEW; }

  /// specify whether ME should be reset at end of monitoring cycle (default:false);
  /// (typically called by Sources that control the original ME)
/* unused */
  void setResetMe(bool /* flag */)
    { data_.flags |= DQMNet::DQM_PROP_RESET; }

  /// true if ME is meant to be stored for each luminosity section
/* unused */
  bool getLumiFlag() const
    { return data_.flags & DQMNet::DQM_PROP_LUMI; }

  /// this ME is meant to be stored for each luminosity section
  void setLumiFlag()
    { data_.flags |= DQMNet::DQM_PROP_LUMI; }

  /// this ME is meant to be an efficiency plot that must not be
  /// normalized when drawn in the DQM GUI.
  void setEfficiencyFlag()
    { data_.flags |= DQMNet::DQM_PROP_EFFICIENCY_PLOT; }

  // A static assert to check that T actually fits in
  // int64_t.
  template <typename T>
  struct fits_in_int64_t
  {
    int checkArray[sizeof(int64_t) - sizeof(T) + 1];
  };

  void Fill(long long x) { fits_in_int64_t<long long>(); doFill(static_cast<int64_t>(x)); }
  void Fill(unsigned long long x) { fits_in_int64_t<unsigned long long>(); doFill(static_cast<int64_t>(x)); }
  void Fill(unsigned long x) { fits_in_int64_t<unsigned long>(); doFill(static_cast<int64_t>(x)); }
  void Fill(long x) { fits_in_int64_t<long>(); doFill(static_cast<int64_t>(x)); }
  void Fill(unsigned int x) { fits_in_int64_t<unsigned int>(); doFill(static_cast<int64_t>(x)); }
  void Fill(int x) { fits_in_int64_t<int>(); doFill(static_cast<int64_t>(x)); }
  void Fill(short x) { fits_in_int64_t<short>(); doFill(static_cast<int64_t>(x)); }
  void Fill(unsigned short x) { fits_in_int64_t<unsigned short>(); doFill(static_cast<int64_t>(x)); }
/* almost unused */   void Fill(char x) { fits_in_int64_t<char>(); doFill(static_cast<int64_t>(x)); }
  void Fill(unsigned char x) { fits_in_int64_t<unsigned char>(); doFill(static_cast<int64_t>(x)); }

  void Fill(float x)    { Fill(static_cast<double>(x)); }
  void Fill(double x);
  void Fill(std::string &value);

  void Fill(double x, double yw);
  void Fill(double x, double y, double zw);
  void Fill(double x, double y, double z, double w);
  void ShiftFillLast(double y, double ye = 0., int32_t xscale = 1);
  void Reset();

  std::string valueString() const;
/* almost unused */   std::string tagString() const;
/* almost unused */   std::string tagLabelString() const;
/* almost unused */   std::string effLabelString() const;
/* almost unused */   std::string qualityTagString(const DQMNet::QValue &qv) const;
  void packScalarData(std::string &into, const char *prefix) const;
/* almost unused */   void packQualityData(std::string &into) const;

  /// true if at least of one of the quality tests returned an error
  bool hasError() const
    { return data_.flags & DQMNet::DQM_PROP_REPORT_ERROR; }

  /// true if at least of one of the quality tests returned a warning
/* unused */
  bool hasWarning() const
    { return data_.flags & DQMNet::DQM_PROP_REPORT_WARN; }

  /// true if at least of one of the tests returned some other (non-ok) status
/* unused */
  bool hasOtherReport() const
    { return data_.flags & DQMNet::DQM_PROP_REPORT_OTHER; }

    /// true if the plot has been marked as an efficiency plot, which
    /// will not be normalized when rendered within the DQM GUI.
/* unused */
/* almost unused */   bool isEfficiency() const
    { return data_.flags & DQMNet::DQM_PROP_EFFICIENCY_PLOT; }

  /// get QReport corresponding to <qtname> (null pointer if QReport does not exist)
  const QReport *getQReport(const std::string &qtname) const;

  /// get map of QReports
  std::vector<QReport *> getQReports() const;

  /// get warnings from last set of quality tests
  std::vector<QReport *> getQWarnings() const;

  /// get errors from last set of quality tests
  std::vector<QReport *> getQErrors() const;

  /// get "other" (i.e. non-error, non-warning, non-"ok") QReports
  /// from last set of quality tests
  std::vector<QReport *> getQOthers() const;

  /// run all quality tests
/* almost unused */   void runQTests();

private:
  void doFill(int64_t x);
  void incompatible(const char *func) const;
  TH1 *accessRootObject(const char *func, int reqdim) const;

public:
#if DQM_ROOT_METHODS
  double getMean(int axis = 1) const;
  double getMeanError(int axis = 1) const;
  double getRMS(int axis = 1) const;
  double getRMSError(int axis = 1) const;
  int getNbinsX() const;
  int getNbinsY() const;
  int getNbinsZ() const;
  double getBinContent(int binx) const;
  double getBinContent(int binx, int biny) const;
/* almost unused */   double getBinContent(int binx, int biny, int binz) const;
  double getBinError(int binx) const;
  double getBinError(int binx, int biny) const;
/* almost unused */   double getBinError(int binx, int biny, int binz) const;
  double getEntries() const;
  double getBinEntries(int bin) const;

private:
/* almost unused */   double getYmin() const;
/* almost unused */   double getYmax() const;

public:
  std::string getAxisTitle(int axis = 1) const;
  std::string getTitle() const;
  void setBinContent(int binx, double content);
  void setBinContent(int binx, int biny, double content);
/* almost unused */   void setBinContent(int binx, int biny, int binz, double content);
  void setBinError(int binx, double error);
  void setBinError(int binx, int biny, double error);
/* almost unused */   void setBinError(int binx, int biny, int binz, double error);
  void setBinEntries(int bin, double nentries);
  void setEntries(double nentries);
  void setBinLabel(int bin, const std::string &label, int axis = 1);
  void setAxisRange(double xmin, double xmax, int axis = 1);
  void setAxisTitle(const std::string &title, int axis = 1);
  void setAxisTimeDisplay(int value, int axis = 1);
  void setAxisTimeFormat(const char *format = "", int axis = 1);

private:
  void setAxisTimeOffset(double toffset, const char *option="local", int axis = 1);

public:
  void setTitle(const std::string &title);
#endif // DQM_ROOT_METHODS

private:
  /// whether soft-reset is enabled; default is false
/* unused */
  bool isSoftResetEnabled() const
    { return refvalue_ != nullptr; }

  /// whether ME contents should be accumulated over multiple monitoring periods; default: false
/* unused */
  bool isAccumulateEnabled() const
    { return data_.flags & DQMNet::DQM_PROP_ACCUMULATE; }

  /// true if ME is marked for deletion
/* unused */
/* almost unused */   bool markedToDelete() const
    { return data_.flags & DQMNet::DQM_PROP_MARKTODELETE; }

  /// Mark the object for deletion.
  /// NB: make sure that the following method is not called simultaneously for the same ME
/* unused */
/* almost unused */   void markToDelete()
    { data_.flags |= DQMNet::DQM_PROP_MARKTODELETE; }

private:
  /// reset "was updated" flag
/* unused */
/* almost unused */   void resetUpdate()
    { data_.flags &= ~DQMNet::DQM_PROP_NEW; }

  /// true if ME should be reset at end of monitoring cycle
/* unused */
/* almost unused */   bool resetMe() const
    { return data_.flags & DQMNet::DQM_PROP_RESET; }

  /// if true, will accumulate ME contents (over many periods)
  /// until method is called with flag = false again
/* unused */
/* almost unused */   void setAccumulate(bool /* flag */)
    { data_.flags |= DQMNet::DQM_PROP_ACCUMULATE; }

  TAxis *getAxis(const char *func, int axis) const;

  // ------------ Operations for MEs that are normally never reset ---------
public:
/* almost unused */   void softReset();
private:
/* almost unused */   void disableSoftReset();
  void addProfiles(TProfile *h1, TProfile *h2, TProfile *sum, float c1, float c2);
  void addProfiles(TProfile2D *h1, TProfile2D *h2, TProfile2D *sum, float c1, float c2);
  void copyFunctions(TH1 *from, TH1 *to);
/* almost unused */   void copyFrom(TH1 *from);


  // --- Operations on MEs that are normally reset at end of monitoring cycle ---
  void getQReport(bool create, const std::string &qtname, QReport *&qr, DQMNet::QValue *&qv);
/* almost unused */   void addQReport(const DQMNet::QValue &desc, QCriterion *qc);
/* almost unused */   void addQReport(QCriterion *qc);
  void updateQReportStats();

public:
  TObject *getRootObject() const;
  TH1 *getTH1() const;
  TH1F *getTH1F() const;
  TH1S *getTH1S() const;
  TH1D *getTH1D() const;
  TH2F *getTH2F() const;
  TH2S *getTH2S() const;
  TH2D *getTH2D() const;
  TH3F *getTH3F() const;
  TProfile *getTProfile() const;
  TProfile2D *getTProfile2D() const;

  TObject *getRefRootObject() const;
  TH1 *getRefTH1() const;
  TH1F *getRefTH1F() const;
  TH1S *getRefTH1S() const;
  TH1D *getRefTH1D() const;
  TH2F *getRefTH2F() const;
  TH2S *getRefTH2S() const;
  TH2D *getRefTH2D() const;
  TH3F *getRefTH3F() const;
  TProfile *getRefTProfile() const;
  TProfile2D *getRefTProfile2D() const;

  int64_t getIntValue() const
    {
      assert(kind() == DQM_KIND_INT);
      return scalar_.num;
    }

  double getFloatValue() const
    {
      assert(kind() == DQM_KIND_REAL);
      return scalar_.real;
    }

  const std::string &getStringValue() const
    {
      assert(kind() == DQM_KIND_STRING);
      return scalar_.str;
    }

/* unused */
  DQMNet::TagList getTags() const // DEPRECATED
    {
      DQMNet::TagList tags;
      if (data_.flags & DQMNet::DQM_PROP_TAGGED)
        tags.push_back(data_.tag);
      return tags;
    }

/* unused */
  const uint32_t getTag() const
    { return data_.tag; }

/* unused */
/* almost unused */   const uint32_t run() const {return data_.run;}
/* unused */
/* almost unused */   const uint32_t lumi() const {return data_.lumi;}
/* unused */
/* almost unused */   const uint32_t moduleId() const {return data_.moduleId;}
};

/* Encapsulate of MonitorElement to expose *limited* support for concurrency.
 *
 * ...
 */

#include <mutex>
#include <tbb/spin_mutex.h>

class ConcurrentMonitorElement
{
private:
  mutable MonitorElement* me_;
  mutable tbb::spin_mutex lock_;

public:
/* unused */
  ConcurrentMonitorElement(void) :
    me_(nullptr)
  { }

  explicit ConcurrentMonitorElement(MonitorElement* me) :
    me_(me)
  { }

  // non-copiable
  ConcurrentMonitorElement(ConcurrentMonitorElement const&) = delete;

  // movable
  ConcurrentMonitorElement(ConcurrentMonitorElement && other)
  {
    std::lock_guard<tbb::spin_mutex> guard(other.lock_);
    me_ = other.me_;
    other.me_ = nullptr;
  }

  // not copy-assignable
  ConcurrentMonitorElement& operator=(ConcurrentMonitorElement const&) = delete;

  // move-assignable
  ConcurrentMonitorElement& operator=(ConcurrentMonitorElement && other)
  {
    // FIXME replace with std::scoped_lock once C++17 is available
    std::lock(lock_, other.lock_);
    std::lock_guard<tbb::spin_mutex> ours(lock_, std::adopt_lock);
    std::lock_guard<tbb::spin_mutex> others(other.lock_, std::adopt_lock);
    me_ = other.me_;
    other.me_ = nullptr;
    return *this;
  }

  // nothing to do, we do not own the MonitorElement
  ~ConcurrentMonitorElement(void) = default;

  // expose as a const method to mean that it is concurrent-safe
  template <typename... Args>
  void fill(Args && ... args) const
  {
    std::lock_guard<tbb::spin_mutex> guard(lock_);
    me_->Fill(std::forward<Args>(args)...);
  }

  // expose as a const method to mean that it is concurrent-safe
  void shiftFillLast(double y, double ye = 0., int32_t xscale = 1) const
  {
    std::lock_guard<tbb::spin_mutex> guard(lock_);
    me_->ShiftFillLast(y, ye, xscale);
  }

  // reset the internal pointer
/* unused */
  void reset()
  {
    std::lock_guard<tbb::spin_mutex> guard(lock_);
    me_ = nullptr;
  }

/* unused */
/* almost unused */   operator bool() const
  {
    std::lock_guard<tbb::spin_mutex> guard(lock_);
    return (me_ != nullptr);
  }

  // non-const methods to manipulate axes and titles.
  // these are not concurrent-safe, and should be used only when the underlying
  // MonitorElement is being booked.
/* almost unused */   void setTitle(std::string const& title)
  {
    me_->setTitle(title);
  }

/* almost unused */   void setXTitle(std::string const& title)
  {
    me_->getTH1()->SetXTitle(title.c_str());
  }

  void setXTitle(const char* title)
  {
    me_->getTH1()->SetXTitle(title);
  }

/* almost unused */   void setYTitle(std::string const& title)
  {
    me_->getTH1()->SetYTitle(title.c_str());
  }

  void setYTitle(const char* title)
  {
    me_->getTH1()->SetYTitle(title);
  }

  void setAxisRange(double xmin, double xmax, int axis = 1)
  {
    me_->setAxisRange(xmin, xmax, axis);
  }

  void setAxisTitle(std::string const& title, int axis = 1)
  {
    me_->setAxisTitle(title, axis);
  }

  void setAxisTimeDisplay(int value, int axis = 1)
  {
    me_->setAxisTimeDisplay(value, axis);
  }

  void setAxisTimeFormat(const char* format = "", int axis = 1)
  {
    me_->setAxisTimeFormat(format, axis);
  }

  void setBinLabel(int bin, std::string const& label, int axis = 1)
  {
    me_->setBinLabel(bin, label, axis);
  }

/* unused */
  void enableSumw2()
  {
    me_->getTH1()->Sumw2();
  }

/* unused */
  void disableAlphanumeric()
  {
    me_->getTH1()->GetXaxis()->SetNoAlphanumeric(false);
    me_->getTH1()->GetYaxis()->SetNoAlphanumeric(false);
  }

  void setOption(const char* option) {
    me_->getTH1()->SetOption(option);
  }
};




namespace edm { class DQMHttpSource; class ParameterSet; class ActivityRegistry; class GlobalContext; }
namespace lat { class Regexp; }
namespace dqmstorepb {class ROOTFilePB; class ROOTFilePB_Histo;}

class MonitorElement;
class QCriterion;
class TFile;
class TBufferFile;
class TObject;
class TH1;
class TObjString;
class TH1F;
class TH1S;
class TH1D;
class TH2F;
class TH2S;
class TH2D;
class TH3F;
class TProfile;
class TProfile2D;
class TNamed;

/** Implements RegEx patterns which occur often in a high-performant
    mattern. For all other expressions, the full RegEx engine is used.
    Note: this class can only be used for lat::Regexp::Wildcard-like
    patterns.  */
class fastmatch {
  enum MatchingHeuristicEnum { UseFull, OneStarStart, OneStarEnd, TwoStar };

public:
  fastmatch(std::string fastString);

/* almost unused */   bool match(std::string const& s) const;

private:
  // checks if two strings are equal, starting at the back of the strings
/* almost unused */   bool compare_strings_reverse(std::string const& pattern,
                               std::string const& input) const;
  // checks if two strings are equal, starting at the front of the strings
/* almost unused */   bool compare_strings(std::string const& pattern,
                       std::string const& input) const;

  std::unique_ptr<lat::Regexp> regexp_{nullptr};
  std::string fastString_;
  MatchingHeuristicEnum matching_;
};


class DQMStore {
public:
  enum SaveReferenceTag {
    SaveWithoutReference,
    SaveWithReference,
    SaveWithReferenceForQTest
  };
  enum OpenRunDirs {
    KeepRunDirs,
    StripRunDirs
  };

  class IBooker {
  public:
    friend class DQMStore;

    MonitorElement* bookInt(TString const& name);
    MonitorElement* bookFloat(TString const& name);
    MonitorElement* bookString(TString const& name, TString const& value);
    MonitorElement* book1D(TString const& name, TString const& title, int const nchX, double const lowX, double const highX);
    MonitorElement* book1D(TString const& name, TString const& title, int nchX, float const* xbinsize);
    MonitorElement* book1D(TString const& name, TH1F* object);
/* almost unused */     MonitorElement* book1S(TString const& name, TString const& title, int nchX, double lowX, double highX);
/* almost unused */     MonitorElement* book1S(TString const& name, TH1S* object);
    MonitorElement* book1DD(TString const& name, TString const& title, int nchX, double lowX, double highX);
/* almost unused */     MonitorElement* book1DD(TString const& name, TH1D* object);
    MonitorElement* book2D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY);
    MonitorElement* book2D(TString const& name, TString const& title, int nchX, float const* xbinsize, int nchY, float const* ybinsize);
    MonitorElement* book2D(TString const& name, TH2F* object);
/* almost unused */     MonitorElement* book2S(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY);
/* almost unused */     MonitorElement* book2S(TString const& name, TString const& title, int nchX, float const* xbinsize, int nchY, float const* ybinsize);
/* almost unused */     MonitorElement* book2S(TString const& name, TH2S* object);
    MonitorElement* book2DD(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY);
/* almost unused */     MonitorElement* book2DD(TString const& name, TH2D* object);
    MonitorElement* book3D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, int nchZ, double lowZ, double highZ);
    MonitorElement* book3D(TString const& name, TH3F* object);
    MonitorElement* bookProfile(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, char const* option = "s");
    MonitorElement* bookProfile(TString const& name, TString const& title, int nchX, double lowX, double highX, double lowY, double highY, char const* option = "s");
/* almost unused */     MonitorElement* bookProfile(TString const& name, TString const& title, int nchX, double const* xbinsize, int nchY, double lowY, double highY, char const* option = "s");
    MonitorElement* bookProfile(TString const& name, TString const& title, int nchX, double const* xbinsize, double lowY, double highY, char const* option = "s");
    MonitorElement* bookProfile(TString const& name, TProfile* object);
    MonitorElement* bookProfile2D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, double lowZ, double highZ, char const* option = "s");
    MonitorElement* bookProfile2D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, int nchZ, double lowZ, double highZ, char const* option = "s");
    MonitorElement* bookProfile2D(TString const& name, TProfile2D* object);

    void cd();
    void cd(std::string const& dir);
    void setCurrentFolder(std::string const& fullpath);
    void goUp();
    std::string const& pwd();
    void tag(MonitorElement*, unsigned int);
/* almost unused */     void tagContents(std::string const&, unsigned int);

    IBooker() = delete;
    IBooker(IBooker const&) = delete;

  private:
    explicit IBooker(DQMStore* store) noexcept : owner_{store}
    {
      assert(store);
    }

    // Embedded classes do not natively own a pointer to the embedding
    // class. We therefore need to store a pointer to the main
    // DQMStore instance (owner_).
    DQMStore* owner_;
  };  // IBooker

  class ConcurrentBooker : public IBooker {
  public:
    friend class DQMStore;

/* almost unused */     ConcurrentMonitorElement bookInt(TString const& name);
/* almost unused */     ConcurrentMonitorElement bookFloat(TString const& name);
/* almost unused */     ConcurrentMonitorElement bookString(TString const& name, TString const& value);
    ConcurrentMonitorElement book1D(TString const& name, TString const& title, int const nchX, double const lowX, double const highX);
/* almost unused */     ConcurrentMonitorElement book1D(TString const& name, TString const& title, int nchX, float const* xbinsize);
/* almost unused */     ConcurrentMonitorElement book1D(TString const& name, TH1F* object);
/* almost unused */     ConcurrentMonitorElement book1S(TString const& name, TString const& title, int nchX, double lowX, double highX);
/* almost unused */     ConcurrentMonitorElement book1S(TString const& name, TH1S* object);
    ConcurrentMonitorElement book1DD(TString const& name, TString const& title, int nchX, double lowX, double highX);
/* almost unused */     ConcurrentMonitorElement book1DD(TString const& name, TH1D* object);
    ConcurrentMonitorElement book2D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY);
    ConcurrentMonitorElement book2D(TString const& name, TString const& title, int nchX, float const* xbinsize, int nchY, float const* ybinsize);
/* almost unused */     ConcurrentMonitorElement book2D(TString const& name, TH2F* object);
    ConcurrentMonitorElement book2S(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY);
    ConcurrentMonitorElement book2S(TString const& name, TString const& title, int nchX, float const* xbinsize, int nchY, float const* ybinsize);
/* almost unused */     ConcurrentMonitorElement book2S(TString const& name, TH2S* object);
/* almost unused */     ConcurrentMonitorElement book2DD(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY);
/* almost unused */     ConcurrentMonitorElement book2DD(TString const& name, TH2D* object);
/* almost unused */     ConcurrentMonitorElement book3D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, int nchZ, double lowZ, double highZ);
/* almost unused */     ConcurrentMonitorElement book3D(TString const& name, TH3F* object);
    ConcurrentMonitorElement bookProfile(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, char const* option = "s");
    ConcurrentMonitorElement bookProfile(TString const& name, TString const& title, int nchX, double lowX, double highX, double lowY, double highY, char const* option = "s");
/* almost unused */     ConcurrentMonitorElement bookProfile(TString const& name, TString const& title, int nchX, double const* xbinsize, int nchY, double lowY, double highY, char const* option = "s");
/* almost unused */     ConcurrentMonitorElement bookProfile(TString const& name, TString const& title, int nchX, double const* xbinsize, double lowY, double highY, char const* option = "s");
/* almost unused */     ConcurrentMonitorElement bookProfile(TString const& name, TProfile* object);
/* almost unused */     ConcurrentMonitorElement bookProfile2D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, double lowZ, double highZ, char const* option = "s");
/* almost unused */     ConcurrentMonitorElement bookProfile2D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, int nchZ, double lowZ, double highZ, char const* option = "s");
    ConcurrentMonitorElement bookProfile2D(TString const& name, TProfile2D* object);

    ConcurrentBooker() = delete;
    ConcurrentBooker(ConcurrentBooker const&) = delete;
    ConcurrentBooker(ConcurrentBooker &&) = delete;
    ConcurrentBooker& operator=(ConcurrentBooker const&) = delete;
    ConcurrentBooker& operator=(ConcurrentBooker &&) = delete;

  private:
    explicit ConcurrentBooker(DQMStore* store) noexcept :
      IBooker{store}
    {}

    ~ConcurrentBooker() = default;
  };

  class IGetter {
  public:
    friend class DQMStore;

    // for the supported syntaxes, see the declarations of DQMStore::getContents
    template <typename... Args>
    std::vector<MonitorElement*> getContents(Args&&... args)
    {
      return owner_->getContents(std::forward<Args>(args)...);
    }

    // for the supported syntaxes, see the declarations of DQMStore::removeElement
    template <typename... Args>
    void removeElement(Args&&... args)
    {
      return owner_->removeElement(std::forward<Args>(args)...);
    }

    std::vector<MonitorElement*> getAllContents(std::string const& path,
                                                 uint32_t runNumber = 0,
                                                 uint32_t lumi = 0);
    MonitorElement* get(std::string const& path);

    // same as get, throws an exception if histogram not found
/* almost unused */     MonitorElement* getElement(std::string const& path);

    std::vector<std::string> getSubdirs();
    std::vector<std::string> getMEs();
/* almost unused */     bool containsAnyMonitorable(std::string const& path);
    bool dirExists(std::string const& path);
    void cd();
    void cd(std::string const& dir);
    void setCurrentFolder(std::string const& fullpath);

    IGetter() = delete;
    IGetter(IGetter const&) = delete;

  private:
    explicit IGetter(DQMStore* store) noexcept : owner_{store}
    {
      assert(store);
    }

    // Embedded classes do not natively own a pointer to the embedding
    // class. We therefore need to store a pointer to the main
    // DQMStore instance (owner_).
    DQMStore* owner_;
  }; //IGetter

  // Template function to be used inside each DQM Modules' lambda
  // functions to book MonitorElements into the DQMStore. The function
  // calls whatever user-supplied code via the function f. The latter
  // is passed the instance of the IBooker class (owned by the *only*
  // DQMStore instance), that is capable of booking MonitorElements
  // into the DQMStore via a public API. The central mutex is acquired
  // *before* invoking and automatically released upon returns.
  template <typename iFunc>
/* almost unused */   void bookTransaction(iFunc f, uint32_t run, uint32_t moduleId, bool canSaveByLumi)
  {
    std::lock_guard<std::mutex> guard(book_mutex_);
    /* Set the run number and module id only if multithreading is enabled */
    if (enableMultiThread_) {
      run_ = run;
      moduleId_ = moduleId;
      canSaveByLumi_ = canSaveByLumi;
    }
    IBooker booker{this};
    f(booker);

    /* Reset the run number and module id only if multithreading is enabled */
    if (enableMultiThread_) {
      run_ = 0;
      moduleId_ = 0;
      canSaveByLumi_ = false;
    }
  }

  // Similar function used to book "global" histograms via the
  // ConcurrentMonitorElement interface.
  template <typename iFunc>
  void bookConcurrentTransaction(iFunc f, uint32_t run)
  {
    std::lock_guard<std::mutex> guard(book_mutex_);
    /* Set the run_ member only if enableMultiThread is enabled */
    if (enableMultiThread_) {
      run_ = run;
    }
    ConcurrentBooker booker(this);
    f(booker);

    /* Reset the run_ member only if enableMultiThread is enabled */
    if (enableMultiThread_) {
      run_ = 0;
    }
  }

  // Signature needed in the harvesting where the booking is done in
  // the endJob. No handles to the run there. Two arguments ensure the
  // capability of booking and getting. The method relies on the
  // initialization of run, stream and module ID to 0. The mutex is
  // not needed.
  template <typename iFunc>
  void meBookerGetter(iFunc f)
  {
    IBooker booker{this};
    IGetter getter{this};
    f(booker, getter);
  }

  //-------------------------------------------------------------------------
  // ---------------------- Constructors ------------------------------------
  DQMStore(edm::ParameterSet const& pset, edm::ActivityRegistry&);
  DQMStore(edm::ParameterSet const& pset);
  DQMStore();
  ~DQMStore();

  //-------------------------------------------------------------------------
  void setVerbose(unsigned level);

  // ---------------------- public navigation -------------------------------
  std::string const& pwd() const;
  void cd();
  void cd(std::string const& subdir);
  void setCurrentFolder(std::string const& fullpath);
  void goUp();

  bool dirExists(std::string const& path) const;

  // Conversion class to allow specifications of TString const&,
  // std::string const&, and char const* for the booking functions.
  // Ideally, this will be replaced with std::string_view (what to do
  // about TString?) whenever we move to C++17.
  class char_string {
  public:
    char_string(TString const& str) : data_{str.Data()} {}
    char_string(char const* str) : data_{str} {}
    char_string(std::string const& str) : data_{str} {}
/* unused */
/* almost unused */     operator std::string const&() const { return data_; }
/* unused */
/* almost unused */     operator char const*() const { return data_.c_str(); }
  private:
    std::string data_;
  };

  //-------------------------------------------------------------------------
  // ---------------------- public ME booking -------------------------------
  MonitorElement* bookInt(char_string const& name);
  MonitorElement* bookFloat(char_string const& name);
  MonitorElement* bookString(char_string const& name,
                             char_string const& value);
  MonitorElement* book1D(char_string const& name,
                         char_string const& title,
                         int const nchX, double const lowX, double const highX);
  MonitorElement* book1D(char_string const& name,
                         char_string const& title,
                         int nchX, float const* xbinsize);
  MonitorElement* book1D(char_string const& name, TH1F* h);
/* almost unused */   MonitorElement* book1S(char_string const& name,
                         char_string const& title,
                         int nchX, double lowX, double highX);
  MonitorElement* book1S(char_string const& name, TH1S* h);
  MonitorElement* book1DD(char_string const& name,
                          char_string const& title,
                          int nchX, double lowX, double highX);
  MonitorElement* book1DD(char_string const& name, TH1D* h);
  MonitorElement* book2D(char_string const& name,
                         char_string const& title,
                         int nchX, double lowX, double highX,
                         int nchY, double lowY, double highY);
/* almost unused */   MonitorElement* book2D(char_string const& name,
                         char_string const& title,
                         int nchX, float const* xbinsize,
                         int nchY, float const* ybinsize);
  MonitorElement* book2D(char_string const& name, TH2F* h);
  MonitorElement* book2S(char_string const& name,
                         char_string const& title,
                         int nchX, double lowX, double highX,
                         int nchY, double lowY, double highY);
  MonitorElement* book2S(char_string const& name,
                         char_string const& title,
                         int nchX, float const* xbinsize,
                         int nchY, float const* ybinsize);
  MonitorElement* book2S(char_string const& name, TH2S* h);
  MonitorElement* book2DD(char_string const& name,
                          char_string const& title,
                          int nchX, double lowX, double highX,
                          int nchY, double lowY, double highY);
  MonitorElement* book2DD(char_string const& name, TH2D* h);
/* almost unused */   MonitorElement* book3D(char_string const& name,
                         char_string const& title,
                         int nchX, double lowX, double highX,
                         int nchY, double lowY, double highY,
                         int nchZ, double lowZ, double highZ);
  MonitorElement* book3D(char_string const& name, TH3F* h);
  MonitorElement* bookProfile(char_string const& name,
                              char_string const& title,
                              int nchX, double lowX, double highX,
                              int nchY, double lowY, double highY,
                              char const* option = "s");
  MonitorElement* bookProfile(char_string const& name,
                              char_string const& title,
                              int nchX, double lowX, double highX,
                              double lowY, double highY,
                              char const* option = "s");
/* almost unused */   MonitorElement* bookProfile(char_string const& name,
                              char_string const& title,
                              int nchX, double const* xbinsize,
                              int nchY, double lowY, double highY,
                              char const* option = "s");
/* almost unused */   MonitorElement* bookProfile(char_string const& name,
                              char_string const& title,
                              int nchX, double const* xbinsize,
                              double lowY, double highY,
                              char const* option = "s");
  MonitorElement* bookProfile(char_string const& name, TProfile* h);
  MonitorElement* bookProfile2D(char_string const& name,
                                char_string const& title,
                                int nchX, double lowX, double highX,
                                int nchY, double lowY, double highY,
                                double lowZ, double highZ,
                                char const* option = "s");
  MonitorElement* bookProfile2D(char_string const& name,
                                char_string const& title,
                                int nchX, double lowX, double highX,
                                int nchY, double lowY, double highY,
                                int nchZ, double lowZ, double highZ,
                                char const* option = "s");
  MonitorElement* bookProfile2D(char_string const& name, TProfile2D* h);

  //-------------------------------------------------------------------------
  // ---------------------- public tagging ----------------------------------
  void tag(MonitorElement* me, unsigned int myTag);
/* almost unused */   void tag(std::string const& path, unsigned int myTag);
/* almost unused */   void tagContents(std::string const& path, unsigned int myTag);
/* almost unused */   void tagAllContents(std::string const& path, unsigned int myTag);

  //-------------------------------------------------------------------------
  // ---------------------- public ME getters -------------------------------
  std::vector<std::string> getSubdirs() const;
  std::vector<std::string> getMEs() const;
/* almost unused */   bool containsAnyMonitorable(std::string const& path) const;

  MonitorElement* get(std::string const& path) const;
/* almost unused */   std::vector<MonitorElement*> get(unsigned int tag) const;
  std::vector<MonitorElement*> getContents(std::string const& path) const;
/* almost unused */   std::vector<MonitorElement*> getContents(std::string const& path, unsigned int tag) const;
  void getContents(std::vector<std::string> &into, bool showContents = true) const;

  // ---------------------- softReset methods -------------------------------
  void softReset(MonitorElement* me);
  void disableSoftReset(MonitorElement* me);

  // ---------------------- Public deleting ---------------------------------
  void rmdir(std::string const& fullpath);
  void removeContents();
/* almost unused */   void removeContents(std::string const& dir);
  void removeElement(std::string const& name);
  void removeElement(std::string const& dir, std::string const& name, bool warning = true);

  // ------------------------------------------------------------------------
  // ---------------------- public I/O --------------------------------------
  void save(std::string const& filename,
            std::string const& path = "",
            std::string const& pattern = "",
            std::string const& rewrite = "",
            uint32_t run = 0,
            uint32_t lumi = 0,
            SaveReferenceTag ref = SaveWithReference,
            int minStatus = dqm::qstatus::STATUS_OK,
            std::string const& fileupdate = "RECREATE");
  void savePB(std::string const& filename,
              std::string const& path = "",
              uint32_t run = 0,
              uint32_t lumi = 0);
  bool open(std::string const& filename,
            bool overwrite = false,
            std::string const& path ="",
            std::string const& prepend = "",
            OpenRunDirs stripdirs = KeepRunDirs,
            bool fileMustExist = true);
  bool load(std::string const& filename,
            OpenRunDirs stripdirs = StripRunDirs,
            bool fileMustExist = true);
/* unused */
  bool mtEnabled() { return enableMultiThread_; };


public:
  // -------------------------------------------------------------------------
  // ---------------------- Public print methods -----------------------------
  void showDirStructure() const;

  // ---------------------- Public check options -----------------------------
  bool isCollate() const;

  // -------------------------------------------------------------------------
  // ---------------------- Quality Test methods -----------------------------
  QCriterion* getQCriterion(std::string const& qtname) const;
  QCriterion* createQTest(std::string const& algoname, std::string const& qtname);
/* almost unused */   void useQTest(std::string const& dir, std::string const& qtname);
  int useQTestByMatch(std::string const& pattern, std::string const& qtname);
  void runQTests();
  int getStatus(std::string const& path = "") const;
  void scaleElements();

private:
  // ---------------- Navigation -----------------------
/* almost unused */   bool cdInto(std::string const& path) const;

  // ------------------- Reference ME -------------------------------
/* almost unused */   bool isCollateME(MonitorElement* me) const;

  // ------------------- Private "getters" ------------------------------
/* almost unused */   bool readFilePB(std::string const& filename,
                  bool overwrite = false,
                  std::string const& path ="",
                  std::string const& prepend = "",
                  OpenRunDirs stripdirs = StripRunDirs,
                  bool fileMustExist = true);
/* almost unused */   bool readFile(std::string const& filename,
                bool overwrite = false,
                std::string const& path ="",
                std::string const& prepend = "",
                OpenRunDirs stripdirs = StripRunDirs,
                bool fileMustExist = true);
/* almost unused */   void makeDirectory(std::string const& path);
/* almost unused */   unsigned int readDirectory(TFile* file,
                             bool overwrite,
                             std::string const& path,
                             std::string const& prepend,
                             std::string const& curdir,
                             OpenRunDirs stripdirs);

/* almost unused */   MonitorElement* findObject(uint32_t run,
                             uint32_t lumi,
                             uint32_t moduleId,
                             std::string const& dir,
                             std::string const& name) const;

/* almost unused */   void get_info(dqmstorepb::ROOTFilePB_Histo const&,
                std::string& dirname,
                std::string& objname,
                TObject** obj);

public:
  std::vector<MonitorElement*> getAllContents(std::string const& path,
                                              uint32_t runNumber = 0,
                                              uint32_t lumi = 0) const;
  std::vector<MonitorElement*> getMatchingContents(std::string const& pattern, lat::Regexp::Syntax syntaxType = lat::Regexp::Wildcard) const;

  // lumisection based histograms manipulations
  void cloneLumiHistograms(uint32_t run, uint32_t lumi, uint32_t moduleId);
  void cloneRunHistograms(uint32_t run, uint32_t moduleId);

  void deleteUnusedLumiHistograms(uint32_t run, uint32_t lumi);

  DQMStore(DQMStore const&) = delete;
  DQMStore& operator=(DQMStore const&) = delete;

private:
  // ---------------- Miscellaneous -----------------------------
/* almost unused */   void initializeFrom(const edm::ParameterSet&);
  void reset();
/* almost unused */   void forceReset();
/* almost unused */   void postGlobalBeginLumi(const edm::GlobalContext&);

/* almost unused */   bool extract(TObject* obj, std::string const& dir, bool overwrite, bool collateHistograms);
/* almost unused */   TObject* extractNextObject(TBufferFile&) const;

  // ---------------------- Booking ------------------------------------
  MonitorElement* initialise(MonitorElement* me, std::string const& path);
/* almost unused */   MonitorElement* book_(std::string const& dir,
                        std::string const& name,
                        char const* context);
  template <class HISTO, class COLLATE>
/* almost unused */   MonitorElement* book_(std::string const& dir,
                        std::string const& name,
                        char const* context,
                        int kind, HISTO* h, COLLATE collate);

/* almost unused */   MonitorElement* bookInt_(std::string const& dir, std::string const& name);
/* almost unused */   MonitorElement* bookFloat_(std::string const& dir, std::string const& name);
/* almost unused */   MonitorElement* bookString_(std::string const& dir, std::string const& name, std::string const& value);
/* almost unused */   MonitorElement* book1D_(std::string const& dir, std::string const& name, TH1F* h);
/* almost unused */   MonitorElement* book1S_(std::string const& dir, std::string const& name, TH1S* h);
/* almost unused */   MonitorElement* book1DD_(std::string const& dir, std::string const& name, TH1D* h);
/* almost unused */   MonitorElement* book2D_(std::string const& dir, std::string const& name, TH2F* h);
/* almost unused */   MonitorElement* book2S_(std::string const& dir, std::string const& name, TH2S* h);
/* almost unused */   MonitorElement* book2DD_(std::string const& dir, std::string const& name, TH2D* h);
/* almost unused */   MonitorElement* book3D_(std::string const& dir, std::string const& name, TH3F* h);
/* almost unused */   MonitorElement* bookProfile_(std::string const& dir, std::string const& name, TProfile* h);
/* almost unused */   MonitorElement* bookProfile2D_(std::string const& dir, std::string const& name, TProfile2D* h);

/* almost unused */   static bool checkBinningMatches(MonitorElement* me, TH1* h, unsigned verbose);

/* almost unused */   static void collate1D(MonitorElement* me, TH1F* h, unsigned verbose);
/* almost unused */   static void collate1S(MonitorElement* me, TH1S* h, unsigned verbose);
/* almost unused */   static void collate1DD(MonitorElement* me, TH1D* h, unsigned verbose);
/* almost unused */   static void collate2D(MonitorElement* me, TH2F* h, unsigned verbose);
/* almost unused */   static void collate2S(MonitorElement* me, TH2S* h, unsigned verbose);
/* almost unused */   static void collate2DD(MonitorElement* me, TH2D* h, unsigned verbose);
/* almost unused */   static void collate3D(MonitorElement* me, TH3F* h, unsigned verbose);
/* almost unused */   static void collateProfile(MonitorElement* me, TProfile* h, unsigned verbose);
/* almost unused */   static void collateProfile2D(MonitorElement* me, TProfile2D* h, unsigned verbose);

  // --- Operations on MEs that are normally reset at end of monitoring cycle ---
/* almost unused */   void setAccumulate(MonitorElement* me, bool flag);

/* almost unused */   void print_trace(std::string const& dir, std::string const& name);

  //-------------------------------------------------------------------------------
  //-------------------------------------------------------------------------------
  using QTestSpec = std::pair<fastmatch*, QCriterion*>;
  using QTestSpecs = std::list<QTestSpec>;
  using MEMap = std::set<MonitorElement>;
  using QCMap = std::map<std::string, QCriterion*>;
  using QAMap = std::map<std::string, QCriterion* (*)(std::string const&)>;

  // ------------------------ private I/O helpers ------------------------------
/* almost unused */   void saveMonitorElementToPB(MonitorElement const& me,
                              dqmstorepb::ROOTFilePB& file);
/* almost unused */   void saveMonitorElementRangeToPB(std::string const& dir,
                                   unsigned int run,
                                   MEMap::const_iterator begin,
                                   MEMap::const_iterator end,
                                   dqmstorepb::ROOTFilePB& file,
                                   unsigned int& counter);
/* almost unused */   void saveMonitorElementToROOT(MonitorElement const& me,
                                TFile& file);
/* almost unused */   void saveMonitorElementRangeToROOT(std::string const& dir,
                                     std::string const& refpath,
                                     SaveReferenceTag ref,
                                     int minStatus,
                                     unsigned int run,
                                     MEMap::const_iterator begin,
                                     MEMap::const_iterator end,
                                     TFile& file,
                                     unsigned int& counter);

  unsigned verbose_{1};
  unsigned verboseQT_{1};
  bool reset_{false};
  double scaleFlag_;
  bool collateHistograms_{false};
  bool enableMultiThread_{false};
  bool LSbasedMode_;
  bool forceResetOnBeginLumi_{false};
  std::string readSelectedDirectory_{};
  uint32_t run_{};
  uint32_t moduleId_{};
  // set to true in the transaction if module supports per-lumi saving.
  bool canSaveByLumi_{false};
  // set to true in configuration if per-lumi saving is requested.
  bool doSaveByLumi_{false};
  std::unique_ptr<std::ostream> stream_{nullptr};

  std::string pwd_{};
  MEMap data_;
  std::set<std::string> dirs_;

  QCMap qtests_;
  QAMap qalgos_;
  QTestSpecs qtestspecs_;

  std::mutex book_mutex_;

  friend class edm::DQMHttpSource;
  friend class DQMService;
  friend class DQMNet;
  friend class DQMArchiver;
  friend class DQMStoreExample; // for get{All,Matching}Contents -- sole user of this method!
  friend class DQMRootOutputModule;
  friend class DQMRootSource;
  friend class DQMFileSaver;
  friend class MEtoEDMConverter;
};

#endif // DQMServices_Core_DQMStore_h

#define CHANGED

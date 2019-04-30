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


#include "DQMServices/Core/interface/ConcurrentMonitorElement.h"

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

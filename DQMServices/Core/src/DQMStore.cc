// silence deprecation warnings for the DQMStore itself.
#define DQM_DEPRECATED
#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <string>
#include <regex>
#include <csignal>

#include <execinfo.h>
#include <cxxabi.h>

namespace dqm::implementation {

  std::string const& NavigatorBase::pwd() { return cwd_; }
  void NavigatorBase::cd() { setCurrentFolder(""); }
  void NavigatorBase::cd(std::string const& dir) { setCurrentFolder(cwd_ + dir); }
  void NavigatorBase::goUp() { cd(".."); }
  void NavigatorBase::setCurrentFolder(std::string const& fullpath) {
    MonitorElementData::Path path;
    path.set(fullpath, MonitorElementData::Path::Type::DIR);
    assert(this);
    cwd_ = path.getDirname();
  }

  IBooker::IBooker(DQMStore* store) {
    store_ = store;
    scope_ = MonitorElementData::Scope::RUN;
  }

  IBooker::~IBooker() {}

  MonitorElementData::Scope IBooker::setScope(MonitorElementData::Scope newscope) {
    auto oldscope = scope_;
    scope_ = newscope;
    return oldscope;
  }

  MonitorElement* IBooker::bookME(TString const& name, MonitorElementData::Kind kind, TH1* object) {
    MonitorElementData* data = new MonitorElementData();
    MonitorElementData::Key key;
    key.kind_ = kind;
    std::string fullpath = pwd() + std::string(name.View());
    key.path_.set(fullpath, MonitorElementData::Path::Type::DIR_AND_NAME);
    key.scope_ = scope_;
    data->key_ = key;
    {
      //MonitorElementData::Value::Access value(data->value_);
      //value.object = std::unique_ptr<TH1>(object);
    }

    std::unique_ptr<MonitorElement> me =
        std::make_unique<MonitorElement>(data, /* is_owned */ true, /* is_readonly */ false);
    assert(me);
    MonitorElement* me_ptr = store_->putME(std::move(me));
    assert(me_ptr);
    return me_ptr;
  }

  MonitorElement* DQMStore::putME(std::unique_ptr<MonitorElement>&& me) {
    //TODO
    return nullptr;
  }

  void DQMStore::printTrace(std::string const& message) {
    edm::LogWarning("DQMStoreBooking").log([&](auto& logger) {
      std::regex s_rxtrace{"(.*)\\((.*)\\+0x.*\\).*(\\[.*\\])"};
      std::regex s_rxself{"^[^()]*dqm::implementation::.*|^[^()]*edm::.*|.*edm::convertException::wrap.*"};

      void* array[10];
      size_t size;
      char** strings;
      int demangle_status = 0;
      std::vector<std::string> clean_trace;

      // glibc/libgcc backtrace functionality, declared in execinfo.h.
      size = backtrace(array, 10);
      strings = backtrace_symbols(array, size);

      size_t level = 1;
      char* demangled = nullptr;
      for (; level < size; ++level) {
        std::cmatch match;
        bool ok = std::regex_match(strings[level], match, s_rxtrace);

        if (!ok) {
          edm::LogWarning("DQMStoreBacktrace") << "failed match" << level << strings[level];
          continue;
        }

        if (match[2].length() == 0) {
          // no symbol, ignore.
          continue;
        }

        // demangle name to human readable form
        demangled = abi::__cxa_demangle(std::string(match[2]).c_str(), nullptr, nullptr, &demangle_status);
        if (!demangled || demangle_status != 0) {
          edm::LogWarning("DQMStoreBacktrace") << "failed demangle! status " << demangle_status << " on " << match[2];
          continue;
        }

        if (std::regex_match(demangled, s_rxself)) {
          // ignore framework/internal methods
          free(demangled);
          demangled = nullptr;
          continue;
        } else {
          // keep the demangled name and the address.
          // The address can be resolved to a line number in gdb attached to
          // the process, using `list *0x<addr>`, but it can only be done in
          // the running process and we can"t easily do it in this code.
          clean_trace.push_back(std::string(demangled) + std::string(match[3]));
          free(demangled);
          demangled = nullptr;
        }
      }

      if (clean_trace.size() > 0) {
        logger << message << " at ";
        for (auto const& s : clean_trace) {
          logger << s << "; ";
        }
      } else {
        logger << message << " : failed to collect stack trace.";
      }

      free(strings);
    });
  }

  MonitorElement* IBooker::bookInt(TString const& name) { return bookME(name, MonitorElementData::Kind::INT, nullptr); }
  MonitorElement* IBooker::bookFloat(TString const& name) {
    return bookME(name, MonitorElementData::Kind::INT, nullptr);
  }
  MonitorElement* IBooker::bookString(TString const& name, TString const& value) {
    return bookME(name, MonitorElementData::Kind::INT, nullptr);
  }
  MonitorElement* IBooker::book1D(
      TString const& name, TString const& title, int const nchX, double const lowX, double const highX) {
    auto th1 = new TH1F(name, title, nchX, lowX, highX);
    return bookME(name, MonitorElementData::Kind::TH1F, th1);
  }
  MonitorElement* IBooker::book1D(TString const& name, TString const& title, int nchX, float const* xbinsize) {
    auto th1 = new TH1F(name, title, nchX, xbinsize);
    return bookME(name, MonitorElementData::Kind::TH1F, th1);
  }
  MonitorElement* IBooker::book1D(TString const& name, TH1F* object) {
    auto th1 = static_cast<TH1*>(object->Clone(name));
    return bookME(name, MonitorElementData::Kind::TH1F, th1);
  }
  MonitorElement* IBooker::book1S(TString const& name, TString const& title, int nchX, double lowX, double highX) {
    auto th1 = new TH1S(name, title, nchX, lowX, highX);
    return bookME(name, MonitorElementData::Kind::TH1S, th1);
  }
  MonitorElement* IBooker::book1S(TString const& name, TH1S* object) {
    auto th1 = static_cast<TH1*>(object->Clone(name));
    return bookME(name, MonitorElementData::Kind::TH1S, th1);
  }
  MonitorElement* IBooker::book1DD(TString const& name, TString const& title, int nchX, double lowX, double highX) {
    auto th1 = new TH1D(name, title, nchX, lowX, highX);
    return bookME(name, MonitorElementData::Kind::TH1D, th1);
  }
  MonitorElement* IBooker::book1DD(TString const& name, TH1D* object) {
    auto th1 = static_cast<TH1*>(object->Clone(name));
    return bookME(name, MonitorElementData::Kind::TH1D, th1);
  }
  MonitorElement* IBooker::book2D(TString const& name,
                                  TString const& title,
                                  int nchX,
                                  double lowX,
                                  double highX,
                                  int nchY,
                                  double lowY,
                                  double highY) {
    auto th2 = new TH2F(name, title, nchX, lowX, highX, nchY, lowY, highY);
    return bookME(name, MonitorElementData::Kind::TH2F, th2);
  }
  MonitorElement* IBooker::book2D(
      TString const& name, TString const& title, int nchX, float const* xbinsize, int nchY, float const* ybinsize) {
    auto th2 = new TH2F(name, title, nchX, xbinsize, nchY, ybinsize);
    return bookME(name, MonitorElementData::Kind::TH2F, th2);
  }
  MonitorElement* IBooker::book2D(TString const& name, TH2F* object) {
    auto th2 = static_cast<TH1*>(object->Clone(name));
    return bookME(name, MonitorElementData::Kind::TH2F, th2);
  }
  MonitorElement* IBooker::book2S(TString const& name,
                                  TString const& title,
                                  int nchX,
                                  double lowX,
                                  double highX,
                                  int nchY,
                                  double lowY,
                                  double highY) {
    auto th2 = new TH2S(name, title, nchX, lowX, highX, nchY, lowY, highY);
    return bookME(name, MonitorElementData::Kind::TH2S, th2);
  }
  MonitorElement* IBooker::book2S(
      TString const& name, TString const& title, int nchX, float const* xbinsize, int nchY, float const* ybinsize) {
    auto th2 = new TH2S(name, title, nchX, xbinsize, nchY, ybinsize);
    return bookME(name, MonitorElementData::Kind::TH2S, th2);
  }
  MonitorElement* IBooker::book2S(TString const& name, TH2S* object) {
    auto th2 = static_cast<TH1*>(object->Clone(name));
    return bookME(name, MonitorElementData::Kind::TH2S, th2);
  }
  MonitorElement* IBooker::book2DD(TString const& name,
                                   TString const& title,
                                   int nchX,
                                   double lowX,
                                   double highX,
                                   int nchY,
                                   double lowY,
                                   double highY) {
    auto th2 = new TH2D(name, title, nchX, lowX, highX, nchY, lowY, highY);
    return bookME(name, MonitorElementData::Kind::TH2D, th2);
  }
  MonitorElement* IBooker::book2DD(TString const& name, TH2D* object) {
    auto th2 = static_cast<TH1*>(object->Clone(name));
    return bookME(name, MonitorElementData::Kind::TH2D, th2);
  }
  MonitorElement* IBooker::book3D(TString const& name,
                                  TString const& title,
                                  int nchX,
                                  double lowX,
                                  double highX,
                                  int nchY,
                                  double lowY,
                                  double highY,
                                  int nchZ,
                                  double lowZ,
                                  double highZ) {
    auto th3 = new TH3F(name, title, nchX, lowX, highX, nchY, lowY, highY, nchZ, lowZ, highZ);
    return bookME(name, MonitorElementData::Kind::TH3F, th3);
  }
  MonitorElement* IBooker::book3D(TString const& name, TH3F* object) {
    auto th3 = static_cast<TH1*>(object->Clone(name));
    return bookME(name, MonitorElementData::Kind::TH3F, th3);
  }
  MonitorElement* IBooker::bookProfile(TString const& name,
                                       TString const& title,
                                       int nchX,
                                       double lowX,
                                       double highX,
                                       int /* nchY */,
                                       double lowY,
                                       double highY,
                                       char const* option) {
    auto tprofile = new TProfile(name, title, nchX, lowX, highX, lowY, highY, option);
    return bookME(name, MonitorElementData::Kind::TPROFILE, tprofile);
  }
  MonitorElement* IBooker::bookProfile(TString const& name,
                                       TString const& title,
                                       int nchX,
                                       double lowX,
                                       double highX,
                                       double lowY,
                                       double highY,
                                       char const* option) {
    auto tprofile = new TProfile(name, title, nchX, lowX, highX, lowY, highY, option);
    return bookME(name, MonitorElementData::Kind::TPROFILE, tprofile);
  }
  MonitorElement* IBooker::bookProfile(TString const& name,
                                       TString const& title,
                                       int nchX,
                                       double const* xbinsize,
                                       int /* nchY */,
                                       double lowY,
                                       double highY,
                                       char const* option) {
    auto tprofile = new TProfile(name, title, nchX, xbinsize, lowY, highY, option);
    return bookME(name, MonitorElementData::Kind::TPROFILE, tprofile);
  }
  MonitorElement* IBooker::bookProfile(TString const& name,
                                       TString const& title,
                                       int nchX,
                                       double const* xbinsize,
                                       double lowY,
                                       double highY,
                                       char const* option) {
    auto tprofile = new TProfile(name, title, nchX, xbinsize, lowY, highY, option);
    return bookME(name, MonitorElementData::Kind::TPROFILE, tprofile);
  }
  MonitorElement* IBooker::bookProfile(TString const& name, TProfile* object) {
    auto tprofile = static_cast<TH1*>(object->Clone(name));
    return bookME(name, MonitorElementData::Kind::TPROFILE, tprofile);
  }
  MonitorElement* IBooker::bookProfile2D(TString const& name,
                                         TString const& title,
                                         int nchX,
                                         double lowX,
                                         double highX,
                                         int nchY,
                                         double lowY,
                                         double highY,
                                         double lowZ,
                                         double highZ,
                                         char const* option) {
    auto tprofile = new TProfile2D(name, title, nchX, lowX, highX, nchY, lowY, highY, lowZ, highZ, option);
    return bookME(name, MonitorElementData::Kind::TPROFILE2D, tprofile);
  }
  MonitorElement* IBooker::bookProfile2D(TString const& name,
                                         TString const& title,
                                         int nchX,
                                         double lowX,
                                         double highX,
                                         int nchY,
                                         double lowY,
                                         double highY,
                                         int /* nchZ */,
                                         double lowZ,
                                         double highZ,
                                         char const* option) {
    auto tprofile = new TProfile2D(name, title, nchX, lowX, highX, nchY, lowY, highY, lowZ, highZ, option);
    return bookME(name, MonitorElementData::Kind::TPROFILE2D, tprofile);
  }
  MonitorElement* IBooker::bookProfile2D(TString const& name, TProfile2D* object) {
    auto tprofile = static_cast<TH1*>(object->Clone(name));
    return bookME(name, MonitorElementData::Kind::TPROFILE2D, tprofile);
  }

  void DQMStore::enterLumi(edm::RunNumber_t run, edm::LuminosityBlockNumber_t lumi, uint64_t moduleID) {
    //TODO
  }

  MonitorElementData* DQMStore::cloneMonitorElementData(MonitorElementData const* input) {
    //TODO
    return nullptr;
  }

  void DQMStore::leaveLumi(edm::RunNumber_t run, edm::LuminosityBlockNumber_t lumi, uint64_t moduleID) {
    //TODO
  }

  std::vector<dqm::harvesting::MonitorElement*> IGetter::getContents(std::string const& path) const { assert(!"NIY"); }
  void IGetter::getContents(std::vector<std::string>& into, bool showContents) const { assert(!"NIY"); }

  std::vector<dqm::harvesting::MonitorElement*> IGetter::getAllContents(std::string const& path) const {
    assert(!"NIY");
  }
  std::vector<dqm::harvesting::MonitorElement*> IGetter::getAllContents(std::string const& path,
                                                                        uint32_t runNumber,
                                                                        uint32_t lumi) const {
    assert(!"NIY");
  }

  MonitorElement* IGetter::get(std::string const& fullpath) const { assert(!"NIY"); }

  MonitorElement* IGetter::getElement(std::string const& path) const { assert(!"NIY"); }

  std::vector<std::string> IGetter::getSubdirs() const { assert(!"NIY"); }
  std::vector<std::string> IGetter::getMEs() const { assert(!"NIY"); }
  bool IGetter::dirExists(std::string const& path) const { assert(!"NIY"); }

  IGetter::IGetter(DQMStore* store) { store_ = store; }

  IGetter::~IGetter() {}

  DQMStore::DQMStore(edm::ParameterSet const& pset, edm::ActivityRegistry&) : IGetter(this), IBooker(this) {}
  DQMStore::~DQMStore() {}

  void DQMStore::save(std::string const& filename,
                      std::string const& path,
                      std::string const& pattern,
                      std::string const& rewrite,
                      uint32_t run,
                      uint32_t lumi,
                      SaveReferenceTag ref,
                      int minStatus,
                      std::string const& fileupdate) {
    assert(!"NIY");
  }
  void DQMStore::savePB(std::string const& filename, std::string const& path, uint32_t run, uint32_t lumi) {
    assert(!"NIY");
  }
  bool DQMStore::open(std::string const& filename,
                      bool overwrite,
                      std::string const& path,
                      std::string const& prepend,
                      OpenRunDirs stripdirs,
                      bool fileMustExist) {
    assert(!"NIY");
  }
  bool DQMStore::load(std::string const& filename, OpenRunDirs stripdirs, bool fileMustExist) { assert(!"NIY"); }

  void DQMStore::showDirStructure() const { assert(!"NIY"); }

  std::vector<MonitorElement*> DQMStore::getMatchingContents(std::string const& pattern) const { assert(!"NIY"); }

}  // namespace dqm::implementation

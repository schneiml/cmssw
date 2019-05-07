#include "DQMServices/Core/interface/Standalone.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/QTest.h"
#include "DQMServices/Core/src/ROOTFilePB.pb.h"
#include "DQMServices/Core/src/DQMError.h"
#include "classlib/utils/RegexpMatch.h"
#include "classlib/utils/Regexp.h"
#include "classlib/utils/StringOps.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/gzip_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include "TFile.h"
#include "TROOT.h"
#include "TKey.h"
#include "TClass.h"
#include "TSystem.h"
#include "TBufferFile.h"
#include <boost/algorithm/string.hpp>
#include <boost/range/iterator_range_core.hpp>

#include <iterator>
#include <cerrno>
#include <exception>
#include <fstream>
#include <sstream>
#include <utility>



/////////////////////////////////////////////////////////////
fastmatch::fastmatch(std::string fastString) :
  fastString_{move(fastString)}, matching_{UseFull}
{
  assert(!"NIY");
}

bool fastmatch::compare_strings_reverse(std::string const& pattern,
                                        std::string const& input) const
{
  assert(!"NIY");
}

bool fastmatch::compare_strings(std::string const& pattern,
                                std::string const& input) const
{
  assert(!"NIY");
}

bool fastmatch::match(std::string const& s) const
{
  assert(!"NIY");
}

//IBooker methods
MonitorElement* DQMStore::IBooker::bookInt(TString const& name)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::bookFloat(TString const& name)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::bookString(TString const& name, TString const& value)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::book1D(TString const& name, TString const& title, int const nchX, double const lowX, double const highX)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::book1D(TString const& name, TString const& title, int nchX, float const* xbinsize)
{
  assert(!"NIY");
};

MonitorElement* DQMStore::IBooker::book1D(TString const& name, TH1F* object)
{
  assert(!"NIY");
}

/* almost unused */ MonitorElement* DQMStore::IBooker::book1S(TString const& name, TString const& title, int nchX, double lowX, double highX)
{
  assert(!"NIY");
}

/* almost unused */ MonitorElement* DQMStore::IBooker::book1S(TString const& name, TH1S* object)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::book1DD(TString const& name, TString const& title, int nchX, double lowX, double highX)
{
  assert(!"NIY");
}

/* almost unused */ MonitorElement* DQMStore::IBooker::book1DD(TString const& name, TH1D* object)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::book2D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::book2D(TString const& name, TString const& title, int nchX, float const* xbinsize, int nchY, float const* ybinsize)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::book2D(TString const& name, TH2F* object)
{
  assert(!"NIY");
}

/* almost unused */ MonitorElement* DQMStore::IBooker::book2S(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY)
{
  assert(!"NIY");
}

/* almost unused */ MonitorElement* DQMStore::IBooker::book2S(TString const& name, TString const& title, int nchX, float const* xbinsize, int nchY, float const* ybinsize)
{
  assert(!"NIY");
}

/* almost unused */ MonitorElement* DQMStore::IBooker::book2S(TString const& name, TH2S* object)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::book2DD(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY)
{
  assert(!"NIY");
}

/* almost unused */ MonitorElement* DQMStore::IBooker::book2DD(TString const& name, TH2D* object)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::book3D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, int nchZ, double lowZ, double highZ)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::book3D(TString const& name, TH3F* object)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::bookProfile(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, char const* option)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::bookProfile(TString const& name, TString const& title, int nchX, double lowX, double highX, double lowY, double highY, char const* option)
{
  assert(!"NIY");
}

/* almost unused */ MonitorElement* DQMStore::IBooker::bookProfile(TString const& name, TString const& title, int nchX, double const* xbinsize, int nchY, double lowY, double highY, char const* option)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::bookProfile(TString const& name, TString const& title, int nchX, double const* xbinsize, double lowY, double highY, char const* option)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::bookProfile(TString const& name, TProfile* object)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::bookProfile2D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, double lowZ, double highZ, char const* option)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::bookProfile2D(TString const& name, TString const& title, int nchX, double lowX, double highX, int nchY, double lowY, double highY, int nchZ, double lowZ, double highZ, char const* option)
{
  assert(!"NIY");
}

MonitorElement* DQMStore::IBooker::bookProfile2D(TString const& name, TProfile2D* object)
{
  assert(!"NIY");
}

void DQMStore::IBooker::cd()
{
  assert(!"NIY");
}

void DQMStore::IBooker::cd(std::string const& dir)
{
  assert(!"NIY");
}

void DQMStore::IBooker::setCurrentFolder(std::string const& fullpath)
{
  assert(!"NIY");
}

void DQMStore::IBooker::goUp()
{
  assert(!"NIY");
}

std::string const& DQMStore::IBooker::pwd()
{
  assert(!"NIY");
}

void DQMStore::IBooker::tag(MonitorElement* me, unsigned int const tag)
{
  assert(!"NIY");
}

void DQMStore::IBooker::tagContents(std::string const& path, unsigned int const myTag)
{
  assert(!"NIY");
}

//IGetter methods
std::vector<MonitorElement*>
DQMStore::IGetter::getAllContents(std::string const& path,
                                  uint32_t const run  /* = 0 */,
                                  uint32_t const lumi /* = 0 */)
{
  assert(!"NIY");
}

MonitorElement*
DQMStore::IGetter::get(std::string const& path)
{
  assert(!"NIY");
}

MonitorElement*
DQMStore::IGetter::getElement(std::string const& path)
{
  assert(!"NIY");
}

std::vector<std::string>
DQMStore::IGetter::getSubdirs()
{
  assert(!"NIY");
}

std::vector<std::string>
DQMStore::IGetter::getMEs()
{
  assert(!"NIY");
}

bool
DQMStore::IGetter::containsAnyMonitorable(std::string const& path)
{
  assert(!"NIY");
}

bool
DQMStore::IGetter::dirExists(std::string const& path)
{
  assert(!"NIY");
}

void
DQMStore::IGetter::cd()
{
  assert(!"NIY");
}

void
DQMStore::IGetter::cd(std::string const& dir)
{
  assert(!"NIY");
}

void
DQMStore::IGetter::setCurrentFolder(std::string const& fullpath)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
DQMStore::DQMStore(edm::ParameterSet const& pset, edm::ActivityRegistry& ar)
  : DQMStore{pset}
{
  assert(!"NIY");
}

DQMStore::DQMStore()
{
  assert(!"NIY");
}

DQMStore::DQMStore(edm::ParameterSet const& pset)
{
  assert(!"NIY");
}

DQMStore::~DQMStore()
{
  assert(!"NIY");
}

void
DQMStore::initializeFrom(edm::ParameterSet const& pset)
{
  assert(!"NIY");
}

/* Generic method to do a backtrace and print it to stdout. It is
   customised to properly get the routine that called the booking of the
   histograms, which, following the usual stack, is at position 4. The
   name of the calling function is properly demangled and the original
   shared library including this function is also printed. For a more
   detailed explanation of the routines involved, see here:
   http://www.gnu.org/software/libc/manual/html_node/Backtraces.html
   http://gcc.gnu.org/onlinedocs/libstdc++/manual/ext_demangling.html.*/

void
DQMStore::print_trace(std::string const& dir, std::string const& name)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// set verbose level (0 turns all non-error messages off)
void
DQMStore::setVerbose(unsigned /* level */)
{}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// return pathname of current directory
std::string const&
DQMStore::pwd() const
{
  assert(!"NIY");
}

/// go to top directory (ie. root)
void
DQMStore::cd()
{
  assert(!"NIY");
}

/// cd to subdirectory (if there)
void
DQMStore::cd(std::string const& subdir)
{
  assert(!"NIY");
}

/// set the last directory in fullpath as the current directory(create if needed);
/// to be invoked by user to specify directories for monitoring objects
/// before booking;
/// commands book1D (etc) & removeElement(name) imply elements in this directory!;
void
DQMStore::setCurrentFolder(std::string const& fullpath)
{
  assert(!"NIY");
}

/// equivalent to "cd .."
void
DQMStore::goUp()
{
  assert(!"NIY");
}

// -------------------------------------------------------------------
/// get folder corresponding to inpath wrt to root (create subdirs if
/// necessary)
void
DQMStore::makeDirectory(std::string const& path)
{
  assert(!"NIY");
}

/// true if directory exists
bool
DQMStore::dirExists(std::string const& path) const
{
  assert(!"NIY");
}

// //====================================================
// // Global-histogram booking
// MonitorElement*
// DQMStore::bookInt(char_string const& name)
// {
//   return bookInt(0, 0, pwd_, name);
// }

// MonitorElement*
// DQMStore::bookFloat(char_string const& name)
// {
//   return bookFloat(0, 0, pwd_, name);
// }

// MonitorElement*
// DQMStore::bookString(char_string const& name,
//                      char_string const& value)
// {
//   return bookString(0, 0, pwd_, name, value);
// }

// MonitorElement*
// DQMStore::book1D(char_string const& name,
//                  char_string const& title,
//                  int const nchX, double const lowX, double const highX)
// {
//   return book1D(0, 0,  pwd_, name, title, nchX, lowX, highX);
// }

// MonitorElement*
// DQMStore::book1D(char_string const& name,
//                  char_string const& title,
//                  int const nchX, float const* xbinsize)
// {
//   return book1D(0, 0,  pwd_, name, title, nchX, xbinsize);
// }

// MonitorElement*
// DQMStore::book1D(char_string const& name, TH1F* h)
// {
//   return book1D(0, 0, pwd_, name, h);
// }

// MonitorElement*
// DQMStore::book1S(char_string const& name,
//                  char_string const& title,
//                  int const nchX, double const lowX, double const highX)
// {
//   return book1S(0, 0,  pwd_, name, title, nchX, lowX, highX);
// }

// MonitorElement*
// DQMStore::book1S(char_string const& name,
//                  char_string const& title,
//                  int const nchX, float const* xbinsize)
// {
//   return book1S(0, 0,  pwd_, name, title, nchX, xbinsize);
// }

// MonitorElement*
// DQMStore::book1S(char_string const& name, TH1S* h)
// {
//   return book1S(0, 0, pwd_, name, h);
// }

// MonitorElement*
// DQMStore::book1DD(char_string const& name,
//                   char_string const& title,
//                   int const nchX, double const lowX, double const highX)
// {
//   return book1DD(0, 0,  pwd_, name, title, nchX, lowX, highX);
// }

// MonitorElement*
// DQMStore::book1DD(char_string const& name,
//                   char_string const& title,
//                   int const nchX, float const* xbinsize)
// {
//   return book1DD(0, 0,  pwd_, name, title, nchX, xbinsize);
// }

// MonitorElement*
// DQMStore::book1DD(char_string const& name, TH1D* h)
// {
//   return book1DD(0, 0, pwd_, name, h);
// }

// MonitorElement*
// DQMStore::book2D(char_string const& name,
//                  char_string const& title,
//                  int const nchX, double const lowX, double const highX,
//                  int const nchY, double const lowY, double const highY)
// {
//   return book2D(0, 0,  pwd_, name, title, nchX, lowX, highX, nchY, lowY, highY);
// }

// MonitorElement*
// DQMStore::book2D(char_string const& name,
//                  char_string const& title,
//                  int const nchX, float const* xbinsize,
//                  int const nchY, float const* ybinsize)
// {
//   return book2D(0, 0,  pwd_, name, title, nchX, xbinsize, nchY, ybinsize);
// }

// MonitorElement*
// DQMStore::book2D(char_string const& name, TH2F* h)
// {
//   return book2D(0, 0, pwd_, name, h);
// }

// MonitorElement*
// DQMStore::book2S(char_string const& name,
//                  char_string const& title,
//                  int const nchX, double const lowX, double const highX,
//                  int const nchY, double const lowY, double const highY)
// {
//   return book2S(0, 0,  pwd_, name, title, nchX, lowX, highX, nchY, lowY, highY);
// }

// MonitorElement*
// DQMStore::book2S(char_string const& name,
//                  char_string const& title,
//                  int const nchX, float const* xbinsize,
//                  int const nchY, float const* ybinsize)
// {
//   return book2S(0, 0,  pwd_, name, title, nchX, xbinsize, nchY, ybinsize);
// }

// MonitorElement*
// DQMStore::book2S(char_string const& name, TH2S* h)
// {
//   return book2S(0, 0, pwd_, name, h);
// }

// MonitorElement*
// DQMStore::book2DD(char_string const& name,
//                   char_string const& title,
//                   int const nchX, double const lowX, double const highX,
//                   int const nchY, double const lowY, double const highY)
// {
//   return book2DD(0, 0,  pwd_, name, title, nchX, lowX, highX, nchY, lowY, highY);
// }

// MonitorElement*
// DQMStore::book2DD(char_string const& name,
//                   char_string const& title,
//                   int const nchX, float const* xbinsize,
//                   int const nchY, float const* ybinsize)
// {
//   return book2DD(0, 0,  pwd_, name, title, nchX, xbinsize, nchY, ybinsize);
// }

// MonitorElement*
// DQMStore::book2DD(char_string const& name, TH2D* h)
// {
//   return book2DD(0, 0, pwd_, name, h);
// }

// MonitorElement*
// DQMStore::book3D(char_string const& name,
//                  char_string const& title,
//                  int const nchX, double const lowX, double const highX,
//                  int const nchY, double const lowY, double const highY,
//                  int const nchZ, double const lowZ, double const highZ)
// {
//   return book3D(0, 0,  pwd_, name, title,
//                 nchX, lowX, highX,
//                 nchY, lowY, highY,
//                 nchZ, lowZ, highZ);
// }

// MonitorElement*
// DQMStore::book3D(char_string const& name, TH3F* h)
// {
//   return book3D(0, 0, pwd_, name, h);
// }

// MonitorElement*
// DQMStore::bookProfile(char_string const& name,
//                       char_string const& title,
//                       int const nchX, double const lowX, double const highX,
//                       int const nchY, double const lowY, double const highY,
//                       char const* option)
// {
//   return bookProfile(0, 0,  pwd_, name, title, nchX, lowX, highX, nchY, lowY, highY, option);
// }

// MonitorElement*
// DQMStore::bookProfile(char_string const& name,
//                       char_string const& title,
//                       int const nchX, double const lowX, double const highX,
//                       double const lowY, double const highY,
//                       char const* option)
// {
//   return bookProfile(0, 0,  pwd_, name, title, nchX, lowX, highX, lowY, highY, option);
// }

// MonitorElement*
// DQMStore::bookProfile(char_string const& name,
//                       char_string const& title,
//                       int const nchX, double const* xbinsize,
//                       int const nchY, double const lowY, double const highY,
//                       char const* option)
// {
//   return bookProfile(0, 0,  pwd_, name, title, nchX, xbinsize, nchY, lowY, highY, option);
// }

// MonitorElement*
// DQMStore::bookProfile(char_string const& name,
//                       char_string const& title,
//                       int const nchX, double const* xbinsize,
//                       double const lowY, double const highY,
//                       char const* option)
// {
//   return bookProfile(0, 0,  pwd_, name, title, nchX, xbinsize, lowY, highY, option);
// }

// MonitorElement*
// DQMStore::bookProfile(char_string const& name, TProfile* h)
// {
//   return bookProfile(0, 0, pwd_, name, h);
// }

// MonitorElement*
// DQMStore::bookProfile2D(char_string const& name,
//                         char_string const& title,
//                         int const nchX, double const lowX, double const highX,
//                         int const nchY, double const lowY, double const highY,
//                         int const nchZ, double const lowZ, double const highZ,
//                         char const* option)
// {
//   return bookProfile2D(0, 0,  pwd_, name, title,
//                        nchX, lowX, highX,
//                        nchY, lowY, highY,
//                        nchZ, lowZ, highZ, option);
// }

// MonitorElement*
// DQMStore::bookProfile2D(char_string const& name,
//                         char_string const& title,
//                         int const nchX, double const lowX, double const highX,
//                         int const nchY, double const lowY, double const highY,
//                         double const lowZ, double const highZ,
//                         char const* option)
// {
//   return bookProfile2D(0, 0,  pwd_, name, title,
//                        nchX, lowX, highX,
//                        nchY, lowY, highY,
//                        lowZ, highZ, option);
// }

// MonitorElement*
// DQMStore::bookProfile2D(char_string const& name, TProfile2D* h)
// {
//   return bookProfile2D(0, 0, pwd_, name, h);
// }


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
template <class HISTO, class COLLATE>
MonitorElement*
DQMStore::book_(std::string const& dir,
                std::string const& name,
                char const* context,
                int const kind,
                HISTO* h,
                COLLATE collate)
{
  assert(!"NIY");
}

MonitorElement*
DQMStore::book_(std::string const& dir,
                std::string const& name,
                char const* context)
{
  assert(!"NIY");
}

// -------------------------------------------------------------------
/// Book int.
MonitorElement*
DQMStore::bookInt_(std::string const& dir, std::string const& name)
{
  assert(!"NIY");
}

/// Book int.
MonitorElement*
DQMStore::bookInt(char_string const& name)
{
  assert(!"NIY");
}

// -------------------------------------------------------------------
/// Book float.
MonitorElement*
DQMStore::bookFloat_(std::string const& dir, std::string const& name)
{
  assert(!"NIY");
}

/// Book float.
MonitorElement*
DQMStore::bookFloat(char_string const& name)
{
  assert(!"NIY");
}

// -------------------------------------------------------------------
/// Book string.
MonitorElement*
DQMStore::bookString_(std::string const& dir,
                      std::string const& name,
                      std::string const& value)
{
  assert(!"NIY");
}

/// Book string.
MonitorElement*
DQMStore::bookString(char_string const& name, char_string const& value)
{
  assert(!"NIY");
}

// -------------------------------------------------------------------
/// Book 1D histogram based on TH1F.
MonitorElement*
DQMStore::book1D_(std::string const& dir, std::string const& name, TH1F* h)
{
  assert(!"NIY");
}

/// Book 1D histogram based on TH1S.
MonitorElement*
DQMStore::book1S_(std::string const& dir, std::string const& name, TH1S* h)
{
  assert(!"NIY");
}

/// Book 1D histogram based on TH1D.
MonitorElement*
DQMStore::book1DD_(std::string const& dir, std::string const& name, TH1D* h)
{
  assert(!"NIY");
}

/// Book 1D histogram.
MonitorElement*
DQMStore::book1D(char_string const& name, char_string const& title,
                 int const nchX, double const lowX, double const highX)
{
  assert(!"NIY");
}

/// Book 1S histogram.
MonitorElement*
DQMStore::book1S(char_string const& name, char_string const& title,
                 int const nchX, double const lowX, double const highX)
{
  assert(!"NIY");
}

/// Book 1S histogram.
MonitorElement*
DQMStore::book1DD(char_string const& name, char_string const& title,
                  int const nchX, double const lowX, double const highX)
{
  assert(!"NIY");
}

/// Book 1D variable bin histogram.
MonitorElement*
DQMStore::book1D(char_string const& name, char_string const& title,
                 int const nchX, const float* xbinsize)
{
  assert(!"NIY");
}

/// Book 1D histogram by cloning an existing histogram.
MonitorElement*
DQMStore::book1D(char_string const& name, TH1F* source)
{
  assert(!"NIY");
}

/// Book 1S histogram by cloning an existing histogram.
MonitorElement*
DQMStore::book1S(char_string const& name, TH1S* source)
{
  assert(!"NIY");
}

/// Book 1D double histogram by cloning an existing histogram.
MonitorElement*
DQMStore::book1DD(char_string const& name, TH1D* source)
{
  assert(!"NIY");
}

// -------------------------------------------------------------------
/// Book 2D histogram based on TH2F.
MonitorElement*
DQMStore::book2D_(std::string const& dir, std::string const& name, TH2F* h)
{
  assert(!"NIY");
}

/// Book 2D histogram based on TH2S.
MonitorElement*
DQMStore::book2S_(std::string const& dir, std::string const& name, TH2S* h)
{
  assert(!"NIY");
}

/// Book 2D histogram based on TH2D.
MonitorElement*
DQMStore::book2DD_(std::string const& dir, std::string const& name, TH2D* h)
{
  assert(!"NIY");
}

/// Book 2D histogram.
MonitorElement*
DQMStore::book2D(char_string const& name, char_string const& title,
                 int const nchX, double const lowX, double const highX,
                 int const nchY, double const lowY, double const highY)
{
  assert(!"NIY");
}

/// Book 2S histogram.
MonitorElement*
DQMStore::book2S(char_string const& name, char_string const& title,
                 int const nchX, double const lowX, double const highX,
                 int const nchY, double const lowY, double const highY)
{
  assert(!"NIY");
}

/// Book 2D histogram.
MonitorElement*
DQMStore::book2DD(char_string const& name, char_string const& title,
                  int const nchX, double const lowX, double const highX,
                  int const nchY, double const lowY, double const highY)
{
  assert(!"NIY");
}

/// Book 2D variable bin histogram.
MonitorElement*
DQMStore::book2D(char_string const& name, char_string const& title,
                 int const nchX, const float* xbinsize, int const nchY, const float* ybinsize)
{
  assert(!"NIY");
}

/// Book 2S variable bin histogram.
MonitorElement*
DQMStore::book2S(char_string const& name, char_string const& title,
                 int const nchX, const float* xbinsize, int const nchY, const float* ybinsize)
{
  assert(!"NIY");
}

/// Book 2D histogram by cloning an existing histogram.
MonitorElement*
DQMStore::book2D(char_string const& name, TH2F* source)
{
  assert(!"NIY");
}

/// Book 2DS histogram by cloning an existing histogram.
MonitorElement*
DQMStore::book2S(char_string const& name, TH2S* source)
{
  assert(!"NIY");
}

/// Book 2DS histogram by cloning an existing histogram.
MonitorElement*
DQMStore::book2DD(char_string const& name, TH2D* source)
{
  assert(!"NIY");
}

// -------------------------------------------------------------------
/// Book 3D histogram based on TH3F.
MonitorElement*
DQMStore::book3D_(std::string const& dir, std::string const& name, TH3F* h)
{
  assert(!"NIY");
}

/// Book 3D histogram.
MonitorElement*
DQMStore::book3D(char_string const& name, char_string const& title,
                 int const nchX, double const lowX, double const highX,
                 int const nchY, double const lowY, double const highY,
                 int const nchZ, double const lowZ, double const highZ)
{
  assert(!"NIY");
}

/// Book 3D histogram by cloning an existing histogram.
MonitorElement*
DQMStore::book3D(char_string const& name, TH3F* source)
{
  assert(!"NIY");
}

// -------------------------------------------------------------------
/// Book profile histogram based on TProfile.
MonitorElement*
DQMStore::bookProfile_(std::string const& dir, std::string const& name, TProfile* h)
{
  assert(!"NIY");
}

/// Book profile.  Option is one of: " ", "s" (default), "i", "G" (see
/// TProfile::BuildOptions).  The number of channels in Y is
/// disregarded in a profile plot.
MonitorElement*
DQMStore::bookProfile(char_string const& name, char_string const& title,
                      int const nchX, double const lowX, double const highX,
                      int /* nchY */, double const lowY, double const highY,
                      char const* option /* = "s" */)
{
  assert(!"NIY");
}

/// Book profile.  Option is one of: " ", "s" (default), "i", "G" (see
/// TProfile::BuildOptions).  The number of channels in Y is
/// disregarded in a profile plot.
MonitorElement*
DQMStore::bookProfile(char_string const& name, char_string const& title,
                      int const nchX, double const lowX, double const highX,
                      double const lowY, double const highY,
                      char const* option /* = "s" */)
{
  assert(!"NIY");
}

/// Book variable bin profile.  Option is one of: " ", "s" (default), "i", "G" (see
/// TProfile::BuildOptions).  The number of channels in Y is
/// disregarded in a profile plot.
MonitorElement*
DQMStore::bookProfile(char_string const& name, char_string const& title,
                      int const nchX, double const* xbinsize,
                      int /* nchY */, double const lowY, double const highY,
                      char const* option /* = "s" */)
{
  assert(!"NIY");
}

/// Book variable bin profile.  Option is one of: " ", "s" (default), "i", "G" (see
/// TProfile::BuildOptions).  The number of channels in Y is
/// disregarded in a profile plot.
MonitorElement*
DQMStore::bookProfile(char_string const& name, char_string const& title,
                      int const nchX, double const* xbinsize,
                      double const lowY, double const highY,
                      char const* option /* = "s" */)
{
  assert(!"NIY");
}

/// Book TProfile by cloning an existing profile.
MonitorElement*
DQMStore::bookProfile(char_string const& name, TProfile* source)
{
  assert(!"NIY");
}

// -------------------------------------------------------------------
/// Book 2D profile histogram based on TProfile2D.
MonitorElement*
DQMStore::bookProfile2D_(std::string const& dir, std::string const& name, TProfile2D* h)
{
  assert(!"NIY");
}

/// Book 2-D profile.  Option is one of: " ", "s" (default), "i", "G"
/// (see TProfile2D::BuildOptions).  The number of channels in Z is
/// disregarded in a 2-D profile.
MonitorElement*
DQMStore::bookProfile2D(char_string const& name, char_string const& title,
                        int const nchX, double const lowX, double const highX,
                        int const nchY, double const lowY, double const highY,
                        int /* nchZ */, double const lowZ, double const highZ,
                        char const* option /* = "s" */)
{
  assert(!"NIY");
}

/// Book 2-D profile.  Option is one of: " ", "s" (default), "i", "G"
/// (see TProfile2D::BuildOptions).  The number of channels in Z is
/// disregarded in a 2-D profile.
MonitorElement*
DQMStore::bookProfile2D(char_string const& name, char_string const& title,
                        int const nchX, double const lowX, double const highX,
                        int const nchY, double const lowY, double const highY,
                        double const lowZ, double const highZ,
                        char const* option /* = "s" */)
{
  assert(!"NIY");
}

/// Book TProfile2D by cloning an existing profile.
MonitorElement*
DQMStore::bookProfile2D(char_string const& name, TProfile2D* source)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
bool
DQMStore::checkBinningMatches(MonitorElement* me, TH1* h, unsigned const verbose)
{
  assert(!"NIY");
}

void
DQMStore::collate1D(MonitorElement* me, TH1F* h, unsigned const verbose)
{
  assert(!"NIY");
}

void
DQMStore::collate1S(MonitorElement* me, TH1S* h, unsigned const verbose)
{
  assert(!"NIY");
}

void
DQMStore::collate1DD(MonitorElement* me, TH1D* h, unsigned const verbose)
{
  assert(!"NIY");
}

void
DQMStore::collate2D(MonitorElement* me, TH2F* h, unsigned const verbose)
{
  assert(!"NIY");
}

void
DQMStore::collate2S(MonitorElement* me, TH2S* h, unsigned const verbose)
{
  assert(!"NIY");
}

void
DQMStore::collate2DD(MonitorElement* me, TH2D* h, unsigned const verbose)
{
  assert(!"NIY");
}

void
DQMStore::collate3D(MonitorElement* me, TH3F* h, unsigned const verbose)
{
  assert(!"NIY");
}

void
DQMStore::collateProfile(MonitorElement* me, TProfile* h, unsigned const verbose)
{
  assert(!"NIY");
}

void
DQMStore::collateProfile2D(MonitorElement* me, TProfile2D* h, unsigned const verbose)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// tag ME as <myTag> (myTag > 0)
void
DQMStore::tag(MonitorElement* me, unsigned int const myTag)
{
  assert(!"NIY");
}

/// tag ME specified by full pathname (e.g. "my/long/dir/my_histo")
void
DQMStore::tag(std::string const& path, unsigned int const myTag)
{
  assert(!"NIY");
}

/// tag all children of folder (does NOT include subfolders)
void
DQMStore::tagContents(std::string const& path, unsigned int const myTag)
{
  assert(!"NIY");
}

/// tag all children of folder, including all subfolders and their children;
/// path must be an exact path name
void
DQMStore::tagAllContents(std::string const& path, unsigned int const myTag)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// get list of subdirectories of current directory
std::vector<std::string>
DQMStore::getSubdirs() const
{
  assert(!"NIY");
}

/// get list of (non-dir) MEs of current directory
std::vector<std::string>
DQMStore::getMEs() const
{
  assert(!"NIY");
}

/// true if directory (or any subfolder at any level below it) contains
/// at least one monitorable element
bool
DQMStore::containsAnyMonitorable(std::string const& path) const
{
  assert(!"NIY");
}

/// get ME from full pathname (e.g. "my/long/dir/my_histo")
MonitorElement*
DQMStore::get(std::string const& path) const
{
  assert(!"NIY");
}

/// get all MonitorElements tagged as <tag>
std::vector<MonitorElement*>
DQMStore::get(unsigned int const tag) const
{
  assert(!"NIY");
}

/// get vector with all children of folder
/// (does NOT include contents of subfolders)
std::vector<MonitorElement*>
DQMStore::getContents(std::string const& path) const
{
  assert(!"NIY");
}

/// same as above for tagged MonitorElements
std::vector<MonitorElement*>
DQMStore::getContents(std::string const& path, unsigned int const tag) const
{
  assert(!"NIY");
}

/// get contents;
/// return vector<string> of the form <dir pathname>:<obj1>,<obj2>,<obj3>;
/// if showContents = false, change form to <dir pathname>:
/// (useful for subscription requests; meant to imply "all contents")
void
DQMStore::getContents(std::vector<std::string>& into, bool const showContents /* = true */) const
{
  assert(!"NIY");
}

/// get MonitorElement <name> in directory <dir>
/// (null if MonitorElement does not exist)
MonitorElement*
DQMStore::findObject(uint32_t const run,
                     uint32_t const lumi,
                     uint32_t const moduleId,
                     std::string const& dir,
                     std::string const& name) const
{
  assert(!"NIY");
}

/// get vector with children of folder, including all subfolders + their children;
/// must use an exact pathname
std::vector<MonitorElement*>
DQMStore::getAllContents(std::string const& path,
                         uint32_t const run  /* = 0 */,
                         uint32_t const lumi /* = 0 */) const
{
  assert(!"NIY");
}

/// get vector with children of folder, including all subfolders + their children;
/// matches names against a wildcard pattern matched against the full ME path
std::vector<MonitorElement*>
DQMStore::getMatchingContents(std::string const& pattern, lat::Regexp::Syntax const syntaxType /* = Wildcard */) const
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Invoke this method after flushing all recently changed monitoring.
    Clears updated flag on all recently updated MEs and calls their
    Reset() method for those that have resetMe = true. */
void
DQMStore::reset()
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Invoke this method after flushing all recently changed monitoring.
    Clears updated flag on all MEs and calls their Reset() method. */
void
DQMStore::forceReset()
{
  assert(!"NIY");
}


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Called after all globalBeginLuminosityBlock.
 * Reset global per-lumi MEs (or all MEs if LSbasedMode) so that
 * they can be reused.
 */
void
DQMStore::postGlobalBeginLumi(edm::GlobalContext const& gc)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Clone the lumisection-based histograms from the 'global' ones
 * (which have lumi = 0) into per-lumi ones (with the lumi number)
 * and reset the global ones.
 * The per-lumi histograms can be saved by the output modules, and
 * will be deleted at the beginninng of the next lumisection.
 */

void
DQMStore::cloneLumiHistograms(uint32_t const run, uint32_t const lumi, uint32_t const moduleId)
{
  assert(!"NIY");
}

/** Same as above, but for run histograms.
 */

void
DQMStore::cloneRunHistograms(uint32_t const run, uint32_t const moduleId)
{
  assert(!"NIY");
}


/** Delete *global* histograms which are no longer in use.
 * Such histograms are created at the end of each lumi and should be
 * deleted after the last globalEndLuminosityBlock.
 */
void
DQMStore::deleteUnusedLumiHistograms(uint32_t const run, uint32_t const lumi)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// extract object (TH1F, TH2F, ...) from <to>; return success flag
/// flag fromRemoteNode indicating if ME arrived from different node
bool
DQMStore::extract(TObject* obj,
                  std::string const& dir,
                  bool const overwrite,
                  bool const collateHistograms)
{
  assert(!"NIY");
}

/// Use this for saving monitoring objects in ROOT files with dir structure;
/// cd into directory (create first if it doesn't exist);
/// returns success flag
bool
DQMStore::cdInto(std::string const& path) const
{
  assert(!"NIY");
}

void
DQMStore::saveMonitorElementToROOT(MonitorElement const& me,
                                   TFile& file)
{
  assert(!"NIY");
}

void
DQMStore::saveMonitorElementRangeToROOT(std::string const& dir,
                                        std::string const& refpath,
                                        SaveReferenceTag const ref,
                                        int const minStatus,
                                        unsigned int const run,
                                        MEMap::const_iterator const begin,
                                        MEMap::const_iterator const end,
                                        TFile& file,
                                        unsigned int& counter)
{
  assert(!"NIY");
}

/// save directory with monitoring objects into protobuf file <filename>;
/// if directory="", save full monitoring structure
void
DQMStore::save(std::string const& filename,
               std::string const& path /* = "" */,
               std::string const& pattern /* = "" */,
               std::string const& rewrite /* = "" */,
               uint32_t const run /* = 0 */,
               uint32_t const lumi /* = 0 */,
               SaveReferenceTag const ref /* = SaveWithReference */,
               int const minStatus /* = dqm::qstatus::STATUS_OK */,
               std::string const& fileupdate /* = RECREATE */)
{
  assert(!"NIY");
}

void
DQMStore::saveMonitorElementToPB(MonitorElement const& me,
                                 dqmstorepb::ROOTFilePB& file)
{
  assert(!"NIY");
}

void
DQMStore::saveMonitorElementRangeToPB(std::string const& dir,
                                      unsigned int const run,
                                      MEMap::const_iterator const begin,
                                      MEMap::const_iterator const end,
                                      dqmstorepb::ROOTFilePB& file,
                                      unsigned int& counter)
{
  assert(!"NIY");
}

/// save directory with monitoring objects into protobuf file <filename>;
/// if directory="", save full monitoring structure
void
DQMStore::savePB(std::string const& filename,
                 std::string const& path /* = "" */,
                 uint32_t const run /* = 0 */,
                 uint32_t const lumi /* = 0 */)
{
  assert(!"NIY");
}


/// read ROOT objects from file <file> in directory <onlypath>;
/// return total # of ROOT objects read
unsigned int
DQMStore::readDirectory(TFile* file,
                        bool const overwrite,
                        std::string const& onlypath,
                        std::string const& prepend,
                        std::string const& curdir,
                        OpenRunDirs const stripdirs)
{
  assert(!"NIY");
}

/// public open/read root file <filename>, and copy MonitorElements;
/// if flag=true, overwrite identical MonitorElements (default: false);
/// if onlypath != "", read only selected directory
/// if prepend !="", prepend string to path
/// note: by default this method keeps the dir structure as in file
/// and does not update monitor element references!
bool
DQMStore::open(std::string const& filename,
               bool const overwrite /* = false */,
               std::string const& onlypath /* ="" */,
               std::string const& prepend /* ="" */,
               OpenRunDirs const stripdirs /* =KeepRunDirs */,
               bool const fileMustExist /* =true */)
{
  assert(!"NIY");
}

/// public load root file <filename>, and copy MonitorElements;
/// overwrite identical MonitorElements (default: true);
/// set DQMStore.collateHistograms to true to sum several files
/// note: by default this method strips off run dir structure
bool
DQMStore::load(std::string const& filename,
               OpenRunDirs const stripdirs /* =StripRunDirs */,
               bool const fileMustExist /* =true */)
{
  assert(!"NIY");
}

/// private readFile <filename>, and copy MonitorElements;
/// if flag=true, overwrite identical MonitorElements (default: false);
/// if onlypath != "", read only selected directory
/// if prepend !="", prepend string to path
/// if StripRunDirs is set the run and run summary folders are erased.
bool
DQMStore::readFile(std::string const& filename,
                   bool const overwrite /* = false */,
                   std::string const& onlypath /* ="" */,
                   std::string const& prepend /* ="" */,
                   OpenRunDirs const stripdirs /* =StripRunDirs */,
                   bool const fileMustExist /* =true */)
{
  assert(!"NIY");
}

/** Extract the next serialised ROOT object from @a buf. Returns null
    if there are no more objects in the buffer, or a null pointer was
    serialised at this location. */
inline TObject* DQMStore::extractNextObject(TBufferFile& buf) const
{
  assert(!"NIY");
}

void DQMStore::get_info(dqmstorepb::ROOTFilePB::Histo const& h,
                        std::string& dirname,
                        std::string& objname,
                        TObject** obj)
{
  assert(!"NIY");
}

bool
DQMStore::readFilePB(std::string const& filename,
                     bool const overwrite /* = false */,
                     std::string const& onlypath /* ="" */,
                     std::string const& prepend /* ="" */,
                     OpenRunDirs const stripdirs /* =StripRunDirs */,
                     bool const fileMustExist /* =true */)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// delete directory and all contents;
/// delete directory (all contents + subfolders);
void
DQMStore::rmdir(std::string const& path)
{
  assert(!"NIY");
}

/// remove all monitoring elements from directory;
void
DQMStore::removeContents(std::string const& dir)
{
  assert(!"NIY");
}

/// erase all monitoring elements in current directory (not including subfolders);
void
DQMStore::removeContents()
{
  assert(!"NIY");
}

/// erase monitoring element in current directory
/// (opposite of book1D,2D,etc. action);
void
DQMStore::removeElement(std::string const& name)
{
  assert(!"NIY");
}

/// remove monitoring element from directory;
/// if warning = true, print message if element does not exist
void
DQMStore::removeElement(std::string const& dir, std::string const& name, bool const warning /* = true */)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// get QCriterion corresponding to <qtname>
/// (null pointer if QCriterion does not exist)
QCriterion*
DQMStore::getQCriterion(std::string const& qtname) const
{
  assert(!"NIY");
}

/// create quality test with unique name <qtname> (analogous to ME name);
/// quality test can then be attached to ME with useQTest method
/// (<algo_name> must match one of known algorithms)
QCriterion*
DQMStore::createQTest(std::string const& algoname, std::string const& qtname)
{
  assert(!"NIY");
}

/// attach quality test <qtname> to directory contents
/// (need exact pathname without wildcards, e.g. A/B/C);
void
DQMStore::useQTest(std::string const& dir, std::string const& qtname)
{
  assert(!"NIY");
}

/// attach quality test <qc> to monitor elements matching <pattern>.
int
DQMStore::useQTestByMatch(std::string const& pattern, std::string const& qtname)
{
  assert(!"NIY");
}
/// run quality tests (also finds updated contents in last monitoring cycle,
/// including newly added content)
void
DQMStore::runQTests()
{
  assert(!"NIY");
}

/// get "global" folder <path> status (one of:STATUS_OK, WARNING, ERROR, OTHER);
/// returns most sever error, where ERROR > WARNING > OTHER > STATUS_OK;
/// see Core/interface/QTestStatus.h for details on "OTHER"
int
DQMStore::getStatus(std::string const& path /* = "" */) const
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/// reset contents (does not erase contents permanently)
/// (makes copy of current contents; will be subtracted from future contents)
void
DQMStore::softReset(MonitorElement* me)
{
  assert(!"NIY");
}

// reverts action of softReset
void
DQMStore::disableSoftReset(MonitorElement* me)
{
  assert(!"NIY");
}

/// if true, will accumulate ME contents (over many periods)
/// until method is called with flag = false again
void
DQMStore::setAccumulate(MonitorElement* me, bool const flag)
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void
DQMStore::showDirStructure() const
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// check if the collate option is active on the DQMStore
bool
DQMStore::isCollate() const
{
  assert(!"NIY");
}
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
// check if the monitor element is in auto-collation folder
bool
DQMStore::isCollateME(MonitorElement* me) const
{
  assert(!"NIY");
}

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/** Invoke this method after flushing all recently changed monitoring.
    Clears updated flag on all MEs and calls their Reset() method. */
void
DQMStore::scaleElements()
{
  assert(!"NIY");
}

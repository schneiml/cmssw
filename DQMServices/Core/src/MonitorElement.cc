#define __STDC_FORMAT_MACROS 1
#define DQM_ROOT_METHODS 1
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/QTest.h"
#include "DQMServices/Core/src/DQMError.h"
#include "TClass.h"
#include "TMath.h"
#include "TList.h"
#include "THashList.h"
#include <iostream>
#include <cassert>
#include <cfloat>
#include <cinttypes>

#if !WITHOUT_CMS_FRAMEWORK
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#endif

static TH1 *checkRootObject(const std::string &name, TObject *tobj, const char *func, int reqdim) { assert(!"NIY"); }

MonitorElement *MonitorElement::initialise(Kind kind) { assert(!"NIY"); }

MonitorElement *MonitorElement::initialise(Kind kind, TH1 *rootobj) { assert(!"NIY"); }

MonitorElement *MonitorElement::initialise(Kind kind, const std::string &value) { assert(!"NIY"); }

MonitorElement::MonitorElement() : object_(nullptr), reference_(nullptr), refvalue_(nullptr) { assert(!"NIY"); }

MonitorElement::MonitorElement(const std::string *path, const std::string &name)
    : object_(nullptr), reference_(nullptr), refvalue_(nullptr) {
  assert(!"NIY");
}

MonitorElement::MonitorElement(const std::string *path, const std::string &name, uint32_t run, uint32_t moduleId)
    : object_(nullptr), reference_(nullptr), refvalue_(nullptr) {
  assert(!"NIY");
}

MonitorElement::MonitorElement(const MonitorElement &x, MonitorElementNoCloneTag)
    : data_(x.data_),
      scalar_(x.scalar_),
      object_(nullptr),
      reference_(x.reference_),
      refvalue_(nullptr),
      qreports_(x.qreports_) {
  assert(!"NIY");
}

MonitorElement::MonitorElement(const MonitorElement &x)
    : MonitorElement::MonitorElement(x, MonitorElementNoCloneTag()) {
  assert(!"NIY");
}

MonitorElement::MonitorElement(MonitorElement &&o) : MonitorElement::MonitorElement(o, MonitorElementNoCloneTag()) {
  assert(!"NIY");
}

MonitorElement::~MonitorElement() { assert(!"NIY"); }

//utility function to check the consistency of the axis labels
//taken from TH1::CheckBinLabels which is not public
bool MonitorElement::CheckBinLabels(const TAxis *a1, const TAxis *a2) { assert(!"NIY"); }

/// "Fill" ME methods for string
void MonitorElement::Fill(std::string &value) const { assert(!"NIY"); }

/// "Fill" ME methods for double
void MonitorElement::Fill(double x) const { assert(!"NIY"); }

/// "Fill" ME method for int64_t
void MonitorElement::doFill(int64_t x) const { assert(!"NIY"); }

/// can be used with 2D (x,y) or 1D (x, w) histograms
void MonitorElement::Fill(double x, double yw) const { assert(!"NIY"); }

/// shift bin to the left and fill last bin with new entry
/// 1st argument is y value, 2nd argument is y error (default 0)
/// can be used with 1D or profile histograms only
void MonitorElement::ShiftFillLast(double y, double ye, int xscale) const { assert(!"NIY"); }
/// can be used with 3D (x, y, z) or 2D (x, y, w) histograms
void MonitorElement::Fill(double x, double y, double zw) const { assert(!"NIY"); }

/// can be used with 3D (x, y, z, w) histograms
void MonitorElement::Fill(double x, double y, double z, double w) const { assert(!"NIY"); }

/// reset ME (ie. contents, errors, etc)
void MonitorElement::Reset() { assert(!"NIY"); }

/// convert scalar data into a string.
void MonitorElement::packScalarData(std::string &into, const char *prefix) const { assert(!"NIY"); }

/// serialise quality report information into a string.
void MonitorElement::packQualityData(std::string &into) const { assert(!"NIY"); }

/// returns value of ME in string format (eg. "f = 3.14151926" for double numbers);
/// relevant only for scalar or string MEs
std::string MonitorElement::valueString() const { assert(!"NIY"); }

/// return tagged value of ME in string format
/// (eg. <name>f=3.14151926</name> for double numbers);
/// relevant only for sending scalar or string MEs over TSocket
std::string MonitorElement::tagString() const { assert(!"NIY"); }

/// return label string for the monitor element tag (eg. <name>t=12345</name>)
std::string MonitorElement::tagLabelString() const { assert(!"NIY"); }

/// return label string for the monitor element tag (eg. <name>t=12345</name>)
std::string MonitorElement::effLabelString() const { assert(!"NIY"); }

std::string MonitorElement::qualityTagString(const DQMNet::QValue &qv) const { assert(!"NIY"); }

const QReport *MonitorElement::getQReport(const std::string &qtname) const { assert(!"NIY"); }

std::vector<QReport *> MonitorElement::getQReports() const { assert(!"NIY"); }

std::vector<QReport *> MonitorElement::getQWarnings() const { assert(!"NIY"); }

std::vector<QReport *> MonitorElement::getQErrors() const { assert(!"NIY"); }

std::vector<QReport *> MonitorElement::getQOthers() const { assert(!"NIY"); }

/// run all quality tests
void MonitorElement::runQTests() { assert(!"NIY"); }

void MonitorElement::incompatible(const char *func) const { assert(!"NIY"); }

TH1 *MonitorElement::accessRootObject(const char *func, int reqdim) const { assert(!"NIY"); }

/*** getter methods (wrapper around ROOT methods) ****/
//
/// get mean value of histogram along x, y or z axis (axis=1, 2, 3 respectively)
double MonitorElement::getMean(int axis /* = 1 */) const {
  return accessRootObject(__PRETTY_FUNCTION__, axis - 1)->GetMean(axis);
}

/// get mean value uncertainty of histogram along x, y or z axis
/// (axis=1, 2, 3 respectively)
double MonitorElement::getMeanError(int axis /* = 1 */) const {
  return accessRootObject(__PRETTY_FUNCTION__, axis - 1)->GetMeanError(axis);
}

/// get RMS of histogram along x, y or z axis (axis=1, 2, 3 respectively)
double MonitorElement::getRMS(int axis /* = 1 */) const {
  return accessRootObject(__PRETTY_FUNCTION__, axis - 1)->GetRMS(axis);
}

/// get RMS uncertainty of histogram along x, y or z axis(axis=1,2,3 respectively)
double MonitorElement::getRMSError(int axis /* = 1 */) const {
  return accessRootObject(__PRETTY_FUNCTION__, axis - 1)->GetRMSError(axis);
}

/// get # of bins in X-axis
int MonitorElement::getNbinsX() const { return accessRootObject(__PRETTY_FUNCTION__, 1)->GetNbinsX(); }

/// get # of bins in Y-axis
int MonitorElement::getNbinsY() const { return accessRootObject(__PRETTY_FUNCTION__, 2)->GetNbinsY(); }

/// get # of bins in Z-axis
int MonitorElement::getNbinsZ() const { return accessRootObject(__PRETTY_FUNCTION__, 3)->GetNbinsZ(); }

/// get content of bin (1-D)
double MonitorElement::getBinContent(int binx) const {
  return accessRootObject(__PRETTY_FUNCTION__, 1)->GetBinContent(binx);
}

/// get content of bin (2-D)
double MonitorElement::getBinContent(int binx, int biny) const {
  return accessRootObject(__PRETTY_FUNCTION__, 2)->GetBinContent(binx, biny);
}

/// get content of bin (3-D)
double MonitorElement::getBinContent(int binx, int biny, int binz) const {
  return accessRootObject(__PRETTY_FUNCTION__, 3)->GetBinContent(binx, biny, binz);
}

/// get uncertainty on content of bin (1-D) - See TH1::GetBinError for details
double MonitorElement::getBinError(int binx) const {
  return accessRootObject(__PRETTY_FUNCTION__, 1)->GetBinError(binx);
}

/// get uncertainty on content of bin (2-D) - See TH1::GetBinError for details
double MonitorElement::getBinError(int binx, int biny) const {
  return accessRootObject(__PRETTY_FUNCTION__, 2)->GetBinError(binx, biny);
}

/// get uncertainty on content of bin (3-D) - See TH1::GetBinError for details
double MonitorElement::getBinError(int binx, int biny, int binz) const {
  return accessRootObject(__PRETTY_FUNCTION__, 3)->GetBinError(binx, biny, binz);
}

/// get # of entries
double MonitorElement::getEntries() const { return accessRootObject(__PRETTY_FUNCTION__, 1)->GetEntries(); }

/// get # of bin entries (for profiles)
double MonitorElement::getBinEntries(int bin) const { assert(!"NIY"); }

/// get min Y value (for profiles)
double MonitorElement::getYmin() const { assert(!"NIY"); }

/// get max Y value (for profiles)
double MonitorElement::getYmax() const { assert(!"NIY"); }

/// get x-, y- or z-axis title (axis=1, 2, 3 respectively)
std::string MonitorElement::getAxisTitle(int axis /* = 1 */) const {
  return getAxis(__PRETTY_FUNCTION__, axis)->GetTitle();
}

/// get MonitorElement title
std::string MonitorElement::getTitle() const { return accessRootObject(__PRETTY_FUNCTION__, 1)->GetTitle(); }

/*** setter methods (wrapper around ROOT methods) ****/
//
/// set content of bin (1-D)
void MonitorElement::setBinContent(int binx, double content) { assert(!"NIY"); }

/// set content of bin (2-D)
void MonitorElement::setBinContent(int binx, int biny, double content) { assert(!"NIY"); }

/// set content of bin (3-D)
void MonitorElement::setBinContent(int binx, int biny, int binz, double content) { assert(!"NIY"); }

/// set uncertainty on content of bin (1-D)
void MonitorElement::setBinError(int binx, double error) { assert(!"NIY"); }

/// set uncertainty on content of bin (2-D)
void MonitorElement::setBinError(int binx, int biny, double error) { assert(!"NIY"); }

/// set uncertainty on content of bin (3-D)
void MonitorElement::setBinError(int binx, int biny, int binz, double error) { assert(!"NIY"); }

/// set # of bin entries (to be used for profiles)
void MonitorElement::setBinEntries(int bin, double nentries) { assert(!"NIY"); }

/// set # of entries
void MonitorElement::setEntries(double nentries) { assert(!"NIY"); }

/// set bin label for x, y or z axis (axis=1, 2, 3 respectively)
void MonitorElement::setBinLabel(int bin, const std::string &label, int axis /* = 1 */) { assert(!"NIY"); }

/// set x-, y- or z-axis range (axis=1, 2, 3 respectively)
void MonitorElement::setAxisRange(double xmin, double xmax, int axis /* = 1 */) { assert(!"NIY"); }

/// set x-, y- or z-axis title (axis=1, 2, 3 respectively)
void MonitorElement::setAxisTitle(const std::string &title, int axis /* = 1 */) { assert(!"NIY"); }

/// set x-, y-, or z-axis to display time values
void MonitorElement::setAxisTimeDisplay(int value, int axis /* = 1 */) { assert(!"NIY"); }

/// set the format of the time values that are displayed on an axis
void MonitorElement::setAxisTimeFormat(const char *format /* = "" */, int axis /* = 1 */) { assert(!"NIY"); }

/// set the time offset, if option = "gmt" then the offset is treated as a GMT time
void MonitorElement::setAxisTimeOffset(double toffset, const char *option /* ="local" */, int axis /* = 1 */) {
  assert(!"NIY");
}

/// set (ie. change) histogram/profile title
void MonitorElement::setTitle(const std::string &title) { assert(!"NIY"); }

TAxis *MonitorElement::getAxis(const char *func, int axis) const { assert(!"NIY"); }

// ------------ Operations for MEs that are normally never reset ---------

/// reset contents (does not erase contents permanently)
/// (makes copy of current contents; will be subtracted from future contents)
void MonitorElement::softReset() { assert(!"NIY"); }

/// reverts action of softReset
void MonitorElement::disableSoftReset() { assert(!"NIY"); }

// implementation: Giuseppe.Della-Ricca@ts.infn.it
// Can be called with sum = h1 or sum = h2
void MonitorElement::addProfiles(TProfile *h1, TProfile *h2, TProfile *sum, float c1, float c2) { assert(!"NIY"); }

// implementation: Giuseppe.Della-Ricca@ts.infn.it
// Can be called with sum = h1 or sum = h2
void MonitorElement::addProfiles(TProfile2D *h1, TProfile2D *h2, TProfile2D *sum, float c1, float c2) {
  assert(!"NIY");
}

void MonitorElement::copyFunctions(TH1 *from, TH1 *to) { assert(!"NIY"); }

void MonitorElement::copyFrom(TH1 *from) { assert(!"NIY"); }

// --- Operations on MEs that are normally reset at end of monitoring cycle ---
void MonitorElement::getQReport(bool create, const std::string &qtname, QReport *&qr, DQMNet::QValue *&qv) {
  assert(!"NIY");
}

/// Add quality report, from DQMStore.
void MonitorElement::addQReport(const DQMNet::QValue &desc, QCriterion *qc) { assert(!"NIY"); }

void MonitorElement::addQReport(QCriterion *qc) { assert(!"NIY"); }

/// Refresh QReport stats, usually after MEs were read in from a file.
void MonitorElement::updateQReportStats() { assert(!"NIY"); }

// -------------------------------------------------------------------
TObject *MonitorElement::getRootObject() const { assert(!"NIY"); }

TH1 *MonitorElement::getTH1() const { assert(!"NIY"); }

TH1F *MonitorElement::getTH1F() const { assert(!"NIY"); }

TH1S *MonitorElement::getTH1S() const { assert(!"NIY"); }

TH1D *MonitorElement::getTH1D() const { assert(!"NIY"); }

TH2F *MonitorElement::getTH2F() const { assert(!"NIY"); }

TH2S *MonitorElement::getTH2S() const { assert(!"NIY"); }

TH2D *MonitorElement::getTH2D() const { assert(!"NIY"); }

TH3F *MonitorElement::getTH3F() const { assert(!"NIY"); }

TProfile *MonitorElement::getTProfile() const { assert(!"NIY"); }

TProfile2D *MonitorElement::getTProfile2D() const { assert(!"NIY"); }

// -------------------------------------------------------------------
TObject *MonitorElement::getRefRootObject() const { assert(!"NIY"); }

TH1 *MonitorElement::getRefTH1() const { assert(!"NIY"); }

TH1F *MonitorElement::getRefTH1F() const { assert(!"NIY"); }

TH1S *MonitorElement::getRefTH1S() const { assert(!"NIY"); }

TH1D *MonitorElement::getRefTH1D() const { assert(!"NIY"); }

TH2F *MonitorElement::getRefTH2F() const { assert(!"NIY"); }

TH2S *MonitorElement::getRefTH2S() const { assert(!"NIY"); }

TH2D *MonitorElement::getRefTH2D() const { assert(!"NIY"); }

TH3F *MonitorElement::getRefTH3F() const { assert(!"NIY"); }

TProfile *MonitorElement::getRefTProfile() const { assert(!"NIY"); }

TProfile2D *MonitorElement::getRefTProfile2D() const { assert(!"NIY"); }

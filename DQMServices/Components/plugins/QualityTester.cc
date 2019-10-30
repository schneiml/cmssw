/*
 * \file QualityTester.cc
 *
 * Helping EDAnalyzer running the quality tests for clients when:
 * - they receive ME data from the SM 
 * - they are run together with the producers (standalone mode)
 *
 * \author M. Zanetti - CERN PH
 *
 */

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "DQMServices/Core/interface/DQMEDHarvester.h"
#include "DQMServices/Core/interface/QTest.h"

#include <cmath>
#include <memory>
#include <string>
#include <cstdio>
#include <sstream>
#include <iostream>

#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>

class QualityTester : public DQMEDHarvester {
public:
  /// Constructor
  QualityTester(const edm::ParameterSet& ps);

  /// Destructor
  ~QualityTester() override;

protected:
  // not called for Harvester for now, might enable that later.
  void analyze(const edm::Event& e, const edm::EventSetup& c) /* override */;

  /// perform the actual quality tests
  void dqmEndLuminosityBlock(DQMStore::IBooker&,
                             DQMStore::IGetter&,
                             edm::LuminosityBlock const& lumiSeg,
                             edm::EventSetup const& c) override;
  void endRun(const edm::Run& r, const edm::EventSetup& c) override;
  void dqmEndJob(DQMStore::IBooker&, DQMStore::IGetter&) override;

private:
  void performTests();

  int nEvents;
  int prescaleFactor;
  bool getQualityTestsFromFile;
  std::string Label;
  bool testInEventloop;
  bool qtestOnEndRun;
  bool qtestOnEndJob;
  bool qtestOnEndLumi;
  std::string reportThreshold;
  bool verboseQT;

  DQMStore* bei;

  // this vector holds and owns all the QTest objects. mostly for memory management.
  std::vector<std::unique_ptr<QCriterion>> qtestobjects;
  // we use this structure to check which tests to run on which MEs. The first
  // part is the pattern with wildcards matching MEs that the test should be
  // applied to, the second points to an object in qtestobjects. We cannot own
  // the objects here since more than one pattern can use the same test.
  std::vector<std::pair<std::string, QCriterion*>> qtestpatterns;

  void configureTests(std::string const& file);
  void attachTests();
  std::unique_ptr<QCriterion> makeQCriterion(boost::property_tree::ptree const& config);
};

QualityTester::QualityTester(const edm::ParameterSet& ps) {
  prescaleFactor = ps.getUntrackedParameter<int>("prescaleFactor", 1);
  getQualityTestsFromFile = ps.getUntrackedParameter<bool>("getQualityTestsFromFile", true);
  Label = ps.getUntrackedParameter<std::string>("label", "");
  reportThreshold = ps.getUntrackedParameter<std::string>("reportThreshold", "");
  testInEventloop = ps.getUntrackedParameter<bool>("testInEventloop", false);
  qtestOnEndRun = ps.getUntrackedParameter<bool>("qtestOnEndRun", true);
  qtestOnEndJob = ps.getUntrackedParameter<bool>("qtestOnEndJob", false);
  qtestOnEndLumi = ps.getUntrackedParameter<bool>("qtestOnEndLumi", false);
  verboseQT = ps.getUntrackedParameter<bool>("verboseQT", true);

  bei = &*edm::Service<DQMStore>();

  if (getQualityTestsFromFile) {
    edm::FileInPath qtlist = ps.getUntrackedParameter<edm::FileInPath>("qtList");
    configureTests(qtlist.fullPath());
  } else {
    assert(!"Reading from DB no longer supported.");
  }

  nEvents = 0;
}

QualityTester::~QualityTester() {}

void QualityTester::analyze(const edm::Event& e, const edm::EventSetup& c) {
  if (testInEventloop) {
    nEvents++;
    if (prescaleFactor > 0 && nEvents % prescaleFactor == 0) {
      performTests();
    }
  }
}

void QualityTester::dqmEndLuminosityBlock(DQMStore::IBooker&,
                                          DQMStore::IGetter&,
                                          edm::LuminosityBlock const& lumiSeg,
                                          edm::EventSetup const& context) {
  if (!testInEventloop && qtestOnEndLumi) {
    if (prescaleFactor > 0 && lumiSeg.id().luminosityBlock() % prescaleFactor == 0) {
      performTests();
    }
  }
}

void QualityTester::endRun(const edm::Run& r, const edm::EventSetup& context) {
  if (qtestOnEndRun)
    performTests();
}

void QualityTester::dqmEndJob(DQMStore::IBooker&, DQMStore::IGetter&) {
  if (qtestOnEndJob)
    performTests();
}

void QualityTester::performTests() {
  // done here because new ME can appear while processing data
  attachTests();

  edm::LogVerbatim("QualityTester") << "Running the Quality Test";

  // TODO: runQTests() on each ME
  // for (auto& kv : this->qtests) {
  //   std::cout << "+++ Qtest " << kv.first << " " << kv.second.qtest.get() <<  "\n";
  //   for (auto& p : kv.second.pathpatterns) {
  //     std::cout << "   +++ at " << p << "\n";
  //   }
  // }

  if (!reportThreshold.empty()) {
    // map {red, orange, black} -> [QReport message, ...]
    std::map<std::string, std::vector<std::string>> theAlarms;
    // populate from MEs hasError, hasWarning, hasOther

    for (auto& theAlarm : theAlarms) {
      const std::string& alarmType = theAlarm.first;
      const std::vector<std::string>& msgs = theAlarm.second;
      if ((reportThreshold == "black") ||
          (reportThreshold == "orange" && (alarmType == "orange" || alarmType == "red")) ||
          (reportThreshold == "red" && alarmType == "red")) {
        std::cout << std::endl;
        std::cout << "Error Type: " << alarmType << std::endl;
        for (auto const& msg : msgs)
          std::cout << msg << std::endl;
      }
    }
    std::cout << std::endl;
  }
}

std::unique_ptr<QCriterion> QualityTester::makeQCriterion(boost::property_tree::ptree const& config) {
  std::map<std::string,
           std::function<std::unique_ptr<QCriterion>(boost::property_tree::ptree const&, std::string& name)>>
      qtestmakers = {
          {ContentsXRange::getAlgoName(),
           [](auto const& config, std::string& name) {
             auto test = std::make_unique<ContentsXRange>(name);
             test->setAllowedXRange(config.template get<double>("xmin"), config.template get<double>("xmax"));
             return test;
           }},
          {ContentsYRange::getAlgoName(),
           [](auto const& config, std::string& name) {
             auto test = std::make_unique<ContentsYRange>(name);
             test->setAllowedYRange(config.template get<double>("ymin"), config.template get<double>("ymax"));
             test->setUseEmptyBins(config.template get<bool>("useEmptyBins"));
             return test;
           }}
          // TODO: add more types
      };

  auto maker = qtestmakers.find(config.get<std::string>("TYPE"));
  // Check if the type is known, error out otherwise.
  if (maker == qtestmakers.end())
    return nullptr;

  // The QTest XML format has structure
  // <QTEST><TYPE>QTestClass</TYPE><PARAM name="thing">value</PARAM>...</QTEST>
  // but that is a pain to read with property_tree. So we reorder the structure
  // and add a child "thing" with data "value" for each param to a new tree.
  // Then the qtestmakers can just config.get<type>("thing").
  boost::property_tree::ptree reordered;
  for (auto kv : config) {
    // TODO: case sensitive?
    if (kv.first == "PARAM") {
      reordered.put(kv.second.get<std::string>("<xmlattr>.name"), kv.second.data());
    }
  }

  auto name = config.get<std::string>("<xmlattr>.name");
  return maker->second(reordered, name);
}

void QualityTester::configureTests(std::string const& file) {
  // We read QTests from the config file into this structure, before
  // transforming into the final [(pattern, *qtest), ...] structure.
  struct TestItem {
    std::unique_ptr<QCriterion> qtest;
    std::vector<std::string> pathpatterns;
  };
  std::map<std::string, TestItem> qtestmap;

  // read XML using property tree. Should throw on error.
  boost::property_tree::ptree xml;
  boost::property_tree::read_xml(file, xml);

  auto it = xml.find("TESTSCONFIGURATION");
  if (it == xml.not_found()) {
    throw cms::Exception("QualityTester") << "QTest XML needs to have a TESTSCONFIGURATION node.";
  }
  auto& testconfig = it->second;
  for (auto& kv : testconfig) {
    // QTEST describes a QTest object (atually QCriterion) with its parameters
    if (kv.first == "QTEST") {
      auto& qtestconfig = kv.second;
      auto name = qtestconfig.get<std::string>("<xmlattr>.name");
      auto value = makeQCriterion(qtestconfig);
      // LINK and QTEST can be in any order, so this element may or may not exist
      qtestmap[name].qtest = std::move(value);
    }  // else
    // LINK associates one ore more QTest referenced by name to a ME path pattern.
    if (kv.first == "LINK") {
      auto& linkconfig = kv.second;
      auto pathpattern = linkconfig.get<std::string>("<xmlattr>.name");
      for (auto& subkv : linkconfig) {
        if (subkv.first == "TestName") {
          std::string testname = subkv.second.data();
          bool enabled = subkv.second.get<bool>("<xmlattr>.activate");
          if (enabled) {
            // LINK and QTEST can be in any order, so this element may or may not exist
            qtestmap[testname].pathpatterns.push_back(pathpattern);
          }
        }
      }
    }
    // else: unknown tag, but that is fine, its XML
  }

  // now that we have all the objects created and references resolved, flatten
  // the structure into something more useful for evaluating tests.
  this->qtestobjects.clear();
  this->qtestpatterns.clear();
  for (auto& kv : qtestmap) {
    QCriterion* bareptr = kv.second.qtest.get();
    for (auto& p : kv.second.pathpatterns) {
      this->qtestpatterns.push_back(std::make_pair(p, bareptr));
    }
    this->qtestobjects.push_back(std::move(kv.second.qtest));
  }
  // we could sort the patterns here to allow more performant matching
  // (using a suffix-array to handle the "*" in the beginning)
}

void QualityTester::attachTests() {}

DEFINE_FWK_MODULE(QualityTester);

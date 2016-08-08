///////////////////////////////////////////////////////////////////////////////
// File: DDCutTubsFromPoints.cc
// Description: Create a ring of CutTubs segments from points on the rim.
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <algorithm>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DetectorDescription/Core/interface/DDCurrentNamespace.h"
#include "DetectorDescription/Core/interface/DDSplit.h"
#include "Geometry/TrackerCommonData/plugins/DDCutTubsFromPoints.h"
#include "CLHEP/Units/GlobalPhysicalConstants.h"
#include "CLHEP/Units/GlobalSystemOfUnits.h"


DDCutTubsFromPoints::DDCutTubsFromPoints() {
  LogDebug("TrackerGeom") <<"DDCutTubsFromPoints info: Creating an instance";
}

DDCutTubsFromPoints::~DDCutTubsFromPoints() {}

void DDCutTubsFromPoints::initialize(const DDNumericArguments & nArgs,
                                   const DDVectorArguments & vArgs,
                                   const DDMapArguments & ,
                                   const DDStringArguments & sArgs,
                                   const DDStringVectorArguments &) {

  r_min = nArgs["rMin"];
  r_max = nArgs["rMax"];
  
  std::vector<double> phis = vArgs["Phi"];
  std::vector<double> z_ls = vArgs["z_l"];
  std::vector<double> z_ts = vArgs["z_t"];

  assert(phis.size() == z_ls.size());
  assert(phis.size() == z_ts.size());

  for (unsigned i = 0; i < phis.size(); i++) {
    Section s = {phis[i], z_ls[i], z_ts[i] };
    sections.push_back(s);
  }

  solidOutput = DDName(sArgs["SolidName"]);
  
  std::string idNameSpace = DDCurrentNamespace::ns();
  DDName parentName = parent().name();
  edm::LogError("TrackerGeom") << "DDCutTubsFromPoints debug: Parent " << parentName
                          << "\tSolid " << solidOutput << " NameSpace " 
                          << idNameSpace 
                          << "\tnumber of sections " << sections.size();
}

void DDCutTubsFromPoints::execute(DDCompactView& cpv) {

}

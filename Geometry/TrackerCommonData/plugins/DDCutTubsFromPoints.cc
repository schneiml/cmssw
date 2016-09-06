///////////////////////////////////////////////////////////////////////////////
// File: DDCutTubsFromPoints.cc
// Description: Create a ring of CutTubs segments from points on the rim.
///////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include <string>
#include <set>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DetectorDescription/Core/interface/DDCurrentNamespace.h"
#include "DetectorDescription/Core/interface/DDSolid.h"
#include "DetectorDescription/Core/interface/DDVector.h"
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
  
  auto phis_name = DDName(sArgs["Phi"]);
  auto z_ls_name = DDName(sArgs["z_l"]);
  auto z_ts_name = DDName(sArgs["z_t"]);
  DDVector phis(phis_name);
  DDVector z_ls(z_ls_name);
  DDVector z_ts(z_ts_name);

  assert(phis.size() == z_ls.size());
  assert(phis.size() == z_ts.size());

  for (unsigned i = 0; i < phis.size(); i++) {
    Section s = {phis[i], z_ls[i], z_ts[i] };
    sections.push_back(s);
  }
  assert(sections.size() > 0);

  solidOutput = DDName(sArgs["SolidName"]);
  
  std::string idNameSpace = DDCurrentNamespace::ns();
  DDName parentName = parent().name();
  LogDebug("TrackerGeom") << "DDCutTubsFromPoints debug: Parent " << parentName
                          << "\tSolid " << solidOutput << " NameSpace " 
                          << idNameSpace 
                          << "\tnumber of sections " << sections.size();
}

static double square(double x) {
  return x*x;
}


void DDCutTubsFromPoints::execute(DDCompactView& cpv) {

  // FIXME: list of segments that cause the "crossing cutted planes" error.
  // These will simply be omitted. We could also try to detect the condition
  // mathematically but since this is a very ugly workaround that should
  // anyways disappear once this is fixed on the Geant side...
  std::set<std::string> blacklist = {
    "PixelForwardInnerDiskOuterRing_seg_1",
    "PixelForwardInnerDiskOuterRing_seg_9",
    "PixelForwardInnerDiskCFOuterRing_seg_1",
    "PixelForwardInnerDiskCFOuterRing_seg_9",
    "PixelForwardInnerDiskInnerRing_seg_7",
    "PixelForwardOuterDiskOuterRing_seg_1",
    "PixelForwardInnerDiskCFInnerRing_seg_7",
    "PixelForwardOuterDiskCFOuterRing_seg_1",
    "PixelForwardOuterDiskOuterRing_seg_9",
    "PixelForwardOuterDiskOuterRing_seg_10",
    "PixelForwardOuterDiskCFOuterRing_seg_9",
    "PixelForwardOuterDiskCFOuterRing_seg_10",
    "PixelForwardOuterDiskInnerRing_seg_1",
    "PixelForwardOuterDiskInnerRing_seg_10",
    "PixelForwardOuterDiskCFInnerRing_seg_1",
    "PixelForwardOuterDiskCFInnerRing_seg_10"
  };

  // radius for plane calculations
  // We use r_max here, since P3 later has a Z that is always more inside
  // than the extreme points. This means the cutting planes have outwards
  // slopes in r-Z, and the corner at r_max could stick out of the bounding
  // volume otherwise. 
  double r  = r_max;

  // min and max z for the placement in the end
  double min_z =  1e9;
  double max_z = -1e9;

  // counter of actually produced segments (excluding skipped ones)
  int segment = 0;

  // the segments and their corresponding offset (absolute, as in the input)
  std::vector<DDSolid> segments;
  std::vector<double>  offsets;
  
  Section s1 = sections[0];
  for (Section s2 : sections) {
    if (s1.phi != s2.phi) {
      segment++;
      // produce segment s1-s2.
      DDName segname  (solidOutput.name() + "_seg_" + std::to_string(segment), 
                       solidOutput.ns());

      double phi1 = s1.phi;
      double phi2 = s2.phi;

      // track the min/max to properly place&align later
      if (s2.z_l < min_z) min_z = s2.z_l;
      if (s2.z_t > max_z) max_z = s2.z_t;

      double P1_z_l = s1.z_l;
      double P1_z_t = s1.z_t;
      double P2_z_l = s2.z_l;
      double P2_z_t = s2.z_t;

      double P1_x_t = cos(phi1) * r;
      double P1_x_l = cos(phi1) * r;
      double P1_y_t = sin(phi1) * r;
      double P1_y_l = sin(phi1) * r;

      double P2_x_t = cos(phi2) * r;
      double P2_x_l = cos(phi2) * r;
      double P2_y_t = sin(phi2) * r;
      double P2_y_l = sin(phi2) * r;

      // each cutting plane is defined by P1-3. P1-2 are corners of the
      // segment, P3 is at r=0 with the "average" z to get a nice cut.
      double P3_z_l = (P1_z_l + P2_z_l) / 2;
      double P3_z_t = (P1_z_t + P2_z_t) / 2;

      // we only have one dz to position both planes. The anchor is implicitly
      // between the P3's, we have to use an offset later to make the segments
      // line up correctly.
      double dz = (P3_z_t - P3_z_l) / 2;
      double offset = (P3_z_t + P3_z_l) / 2;

      // the plane is defined by P1-P3 and P2-P3; since P3 is at r=0 we
      // only need the z.
      double D1_z_l = P1_z_l - P3_z_l;
      double D2_z_l = P2_z_l - P3_z_l;

      // the normal is then the cross product...
      double n_x_l = (P1_y_l * D2_z_l) - (D1_z_l * P2_y_l);
      double n_y_l = (D1_z_l * P2_x_l) - (P1_x_l * D2_z_l);
      double n_z_l = (P1_x_l * P2_y_l) - (P1_y_l * P2_x_l);

      // ... normalized.
      // flip the sign here (but not for t) since root wants it like that.
      double norm = -sqrt(square(n_x_l) + square(n_y_l) + square(n_z_l));
      n_x_l /= norm;
      n_y_l /= norm;
      n_z_l /= norm;
      //double norm_l = sqrt(square(n_x_l) + square(n_y_l) + square(n_z_l));

      // same game for the t side.
      double D1_z_t = P1_z_t - P3_z_t;
      double D2_z_t = P2_z_t - P3_z_t;

      double n_x_t = (P1_y_t * D2_z_t) - (D1_z_t * P2_y_t);
      double n_y_t = (D1_z_t * P2_x_t) - (P1_x_t * D2_z_t);
      double n_z_t = (P1_x_t * P2_y_t) - (P1_y_t * P2_x_t);

      norm = sqrt(square(n_x_t) + square(n_y_t) + square(n_z_t));
      n_x_t /= norm;
      n_y_t /= norm;
      n_z_t /= norm;
      //double norm_t = sqrt(square(n_x_t) + square(n_y_t) + square(n_z_t));

      // the cuttubs wants a delta phi
      double dphi = phi2 - phi1;

      // TODO: make 2 cuttubs here, one with the left, one with the right plane.
      // then intersect for the actual tubs.
      DDSolid seg = DDSolidFactory::cuttubs(segname, dz, r_min, r_max, phi1, dphi,
                                            n_x_l, n_y_l, n_z_l,
                                            n_x_t, n_y_t, n_z_t); 
      if (blacklist.find(segname.name()) == blacklist.end()) {
        segments.push_back(seg);
        offsets.push_back(offset);
      }
      // XML output for debugging
      //std::cout << std::fixed << std::setprecision(12) 
        //<< "+++ <CutTubs name=\"" << segname.name() 
        //<< "\" rMin=\"" << r_min << "\" rMax=\"" << r_max 
        //<< "\" dz=\"" << dz
        //<< "\" startPhi=\"" << phi1 << "\" deltaPhi=\"" << dphi 
        //<< "\" lx=\"" << n_x_l << "\" ly=\"" << n_y_l << "\" lz=\"" << n_z_l 
        //<< "\" tx=\"" << n_x_t << "\" ty=\"" << n_y_t << "\" tz=\"" << n_z_t << "\"/>"
        //<< "<!-- norm: " << norm_l << " " << norm_t << " -->" << "\n";
    }
    s1 = s2;
  }

  assert(segments.size() >= 2); // less would be special cases

  // remove the common offset from the input, to get sth. aligned at z=0.
  double shift = (min_z + (max_z-min_z)/2);
  // this box defines the origin of the full ring
  DDName dummy_name(solidOutput.name() + "_dummy", solidOutput.ns());
  DDSolid dummy = DDSolidFactory::box(dummy_name, r_max, r_max, (max_z-min_z)/2);

  // we place the first segment into the dummy box using intersection; later
  // this will always be the first member of the union and give the right 
  // placement.
  DDName seg0_shifted(solidOutput.name() + "_uni_0", solidOutput.ns());
  DDSolid solid = DDSolidFactory::intersection(seg0_shifted, dummy, segments[0],
    DDTranslation(0, 0, offsets[0] - shift), DDRotation());

  for (unsigned i = 1; i < segments.size()-1; i++) {
    // each sub-union needs a name. Well. 
    DDName unionname(solidOutput.name() + "_uni_" + std::to_string(i+1), 
                     solidOutput.ns());
    solid = DDSolidFactory::unionSolid(unionname, solid, segments[i], 
             DDTranslation(0, 0, offsets[i] - shift), 
             DDRotation());
  }

  // rename the last one to the output by adding the empty box again...
  solid = DDSolidFactory::unionSolid(solidOutput, solid, segments[segments.size()-1], 
            DDTranslation(0, 0, offsets[segments.size()-1] - shift),
            DDRotation());
}

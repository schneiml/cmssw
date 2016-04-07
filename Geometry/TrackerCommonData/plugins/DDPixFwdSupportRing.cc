///////////////////////////////////////////////////////////////////////////////
// File: DDPixFwdSupportRing.cc
// Description: Create Tubs of the support rings from given points connecting them
///////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DetectorDescription/Base/interface/DDutils.h"
#include "DetectorDescription/Core/interface/DDCurrentNamespace.h"
#include "DetectorDescription/Core/interface/DDSplit.h"
#include "Geometry/TrackerCommonData/plugins/DDPixFwdSupportRing.h"
#include "CLHEP/Units/PhysicalConstants.h"
#include "CLHEP/Units/SystemOfUnits.h"

DDPixFwdSupportRing::DDPixFwdSupportRing() {
  LogDebug("TrackerGeom") <<"DDPixFwdSupportRing info: Creating an instance";
}

DDPixFwdSupportRing::~DDPixFwdSupportRing() {}

void DDPixFwdSupportRing::initialize(const DDNumericArguments & nArgs,
				  const DDVectorArguments & vArgs,
				  const DDMapArguments & ,
				  const DDStringArguments & sArgs,
				  const DDStringVectorArguments & vsArgs) {

  angle = vArgs["Angle"];
  zPos  = vArgs["ZPos" ];
  zShift = nArgs["AnchorZ"];
  count = int(nArgs["Count"]);

  otherHalf = bool(nArgs["CreateBothHalves"]);
  
  //The number of linear sections/parts
  partCount = angle.size();


  idNameSpace = DDCurrentNamespace::ns();
  startCopyNo = int(nArgs["StartCopyNo"]);
  child1Name  = sArgs["Child1Name"];
  child2Name  = sArgs["Child2Name"];
  rotName     = sArgs["RotationName"];
  DDName parentName = parent().name();

  LogDebug("TrackerGeom") << "DDPixFwdSupportRing debug: Parent " << parentName 
			  << "\tChild1 " << child1Name << "\tChild2 " << child2Name
			  << " NameSpace " << idNameSpace << "\tRot Name "
			  << rotName << "\tCopyNo (Start/Total) " << startCopyNo
			  << ", " << count  << "\tAnchorZ " << zShift;
  //<< "\tAnchor radius " << anchorR;
  
}

void DDPixFwdSupportRing::execute(DDCompactView& cpv) {
  
  int copy = startCopyNo;
  DDName mother = parent().name();
  DDName child1(DDSplit(child1Name).first, DDSplit(child1Name).second);
  DDName child2(DDSplit(child2Name).first, DDSplit(child2Name).second);

  std::string rotns = DDSplit(rotName).second;

  //Current phi
  double phi = angle[0] + 90. * CLHEP::deg;
  //Current (local) Z shift
  double currZ = zPos[0];
  
  //The whole length in radians
  double L = angle[partCount-1] - angle[0];

  //Phi per Tubs
  double dPhi = L / count;

  //Which section are we in
  int ind = 1;

  //The angle left from the current section
  double left = angle[1] - angle[0];

  for(int tubIndex = 0; tubIndex < count; tubIndex++)
  {
    std::string rotstr = DDSplit(rotName).first + dbl_to_string(double(copy));
    //If the other half ring should be generated too
    std::string rotstr2 = DDSplit(rotName).first + dbl_to_string(double(copy+count));

    //If it fits into the current section / is in the last section
    if( (left >= dPhi) || ( ind == partCount - 1 ))
    {
      left -= dPhi;
      //half the shift before placing (the Tubs' anchor point is in its middle)
      currZ += (zPos[ind] - zPos[ind-1]) / (angle[ind] - angle[ind-1]) * dPhi / 2.0;
      
      //place it
      double xpos = 0;//anchorR * -std::sin(phi);
      double ypos = 0;//anchorR * std::cos(phi);
      double zpos = currZ + zShift;
      
      DDTranslation tran(xpos, ypos, zpos);

      double phix  = phi + dPhi/2.0;
      double thetx = 90. * CLHEP::deg;
      double phiy  = 90. * CLHEP::deg + phi + dPhi / 2.0;
      double thety = 90. * CLHEP::deg;
      double phiz  = 0.0;
      double thetz = 0.0;
      
      DDRotation rot = DDrot(DDName(rotstr, rotns), thetx,phix, thety,phiy, thetz,phiz);

      cpv.position (child1, mother, copy, tran, rot);
      cpv.position (child2, mother, copy, tran, rot);

      //If the other half ring is created too
      if(otherHalf)
      {
	phix  = phi + 180*CLHEP::deg + dPhi/2.0;
	thetx = 90. * CLHEP::deg;
	phiy  = 270. * CLHEP::deg + phi + dPhi / 2.0;
	thety = 90. * CLHEP::deg;
	phiz  = 0.0;
	thetz = 0.0;

	rot = DDrot(DDName(rotstr2, rotns), thetx,phix, thety,phiy, thetz,phiz);
        
	cpv.position (child1, mother, copy+count, tran, rot);
	cpv.position (child2, mother, copy+count, tran, rot);
      }

      //half the shift after placing
      currZ += (zPos[ind] - zPos[ind-1]) / (angle[ind] - angle[ind-1]) * dPhi / 2.0;
    }
    else
    {
      //Moving to the next section
      ind++;

      //half the shift before placing
      currZ += ( (zPos[ind-1] - zPos[ind-2]) / (angle[ind-1] - angle[ind-2]) * left +
		 (zPos[ind] - zPos[ind-1]) / (angle[ind] - angle[ind-1]) * (dPhi - left) ) / 2.0;

      //place it
      double xpos = 0;//anchorR * -std::sin(phi);
      double ypos = 0;//anchorR * std::cos(phi);
      double zpos = currZ + zShift;

      DDTranslation tran(xpos, ypos, zpos);

      double phix  = phi + dPhi / 2.0;
      double thetx = 90. * CLHEP::deg;
      double phiy  = 90. * CLHEP::deg + phi + dPhi / 2.0;
      double thety = 90. * CLHEP::deg;
      double phiz  = 0.0;
      double thetz = 0.0;

      DDRotation rot = DDrot(DDName(rotstr, rotns), thetx,phix, thety,phiy, thetz,phiz);

      cpv.position (child1, mother, copy, tran, rot);
      cpv.position (child2, mother, copy, tran, rot);

      //If the other half ring is created too
      if(otherHalf)
      {
	phix  = phi + 180*CLHEP::deg + dPhi/2.0;
	thetx = 90. * CLHEP::deg;
	phiy  = 270. * CLHEP::deg + phi + dPhi / 2.0;
	thety = 90. * CLHEP::deg;
	phiz  = 0.0;
	thetz = 0.0;

	rot = DDrot(DDName(rotstr2, rotns), thetx,phix, thety,phiy, thetz,phiz);

	cpv.position (child1, mother, copy+count, tran, rot);
	cpv.position (child2, mother, copy+count, tran, rot);
      }


      //half the shift after placing
      currZ += ( (zPos[ind-1] - zPos[ind-2]) / (angle[ind-1] - angle[ind-2]) * left +
		 (zPos[ind] - zPos[ind-1]) / (angle[ind] - angle[ind-1]) * (dPhi - left) ) / 2.0;

      //angle left from the next section
      left = angle[ind] - angle[ind-1] + left - dPhi;
    }

    phi += dPhi;
    copy++;
  }
}

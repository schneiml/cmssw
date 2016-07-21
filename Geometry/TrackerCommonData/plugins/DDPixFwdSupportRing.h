#ifndef DD_PixFwdSupportRing_h
#define DD_PixFwdSupportRing_h

#include <map>
#include <string>
#include <vector>
#include "DetectorDescription/Base/interface/DDTypes.h"
#include "DetectorDescription/Algorithm/interface/DDAlgorithm.h"

class DDPixFwdSupportRing : public DDAlgorithm {
 public:
  //Constructor and Destructor                                                                              
  DDPixFwdSupportRing();
  virtual ~DDPixFwdSupportRing();

  void initialize(const DDNumericArguments & nArgs,
                  const DDVectorArguments & vArgs,
                  const DDMapArguments & mArgs,
                  const DDStringArguments & sArgs,
                  const DDStringVectorArguments & vsArgs);

  void execute(DDCompactView& cpv);

 private:

  std::string              idNameSpace; //Namespace of this and ALL sub-parts                               
  std::string              child1Name;  //Child1 name (RingSegment)                                         
  std::string              child2Name;  //Child2 name (CFSegment)                                           
  std::string              rotName;     //Name of the base rotation matrix                                  
  int                      count;       //Number of Tubs in a half ring
  int                      partCount;   //Number of sections
  int                      startCopyNo; //Start Copy number
  double                   zShift;      //Anchor Z shift
  std::vector<double>      angle;       //Length of each section in degrees (first one is phi0)
  std::vector<double>      zPos;        //Z position of the Tubs at the ends of the sections
  bool                     otherHalf;   //If true creates the other half ring too
  };

#endif


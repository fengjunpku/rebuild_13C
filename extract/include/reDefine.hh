#ifndef REDEFINE_HH
#define REDEFINE_HH 1

#include <TString.h>

using namespace std;

namespace reDef
{
  //static const TString in_dir = "/Volumes/JunData/c13_xd/setefile/";
  //static const TString ou_dir = "/Volumes/JunData/c13_xd/reV1/";
  //static const TString in_dir = "../../setefile/";
  //static const TString ou_dir = "../../reV1/";
  static const TString in_dir = "/data/d2/CIAE_Jan2017_13C/setefile/";
  static const TString ou_dir = "/data/d2/CIAE_Jan2017_13C/extract_pd/";

  static const double Mass_C12 = 11174.9;//*MeV
  static const double Mass_C13 = 12109.5;//*MeV
  static const double Mass_Be8 = 7454.85;//*MeV
  static const double Mass_Be9 = 8392.75;//*MeV
  static const double Mass_He4 = 3727.38;//*MeV
  static const double Mass_Ne20 = 18617.7;//*MeV
  static const double Mass_O16 = 14895.1;//*MeV

  //thickness of dead layer [um]
  static const double DL_l0[3] = {1.25,1.1,1.3};//w1 bb7 ssd
  static const double DL_r0[3] = {0.75,0.8,0.7};//w1 bb7 ssd
  static const double DL_l1[2] = {1,1};//w1 ssd
  static const double DL_r1[2] = {1,1};//w1 ssd

  //thickness if target
  static const double halfTT = 0.5;//um 9Be

  //tele th ph
  static const double angles_l0[2] = {24 , 0  };
  static const double angles_r0[2] = {24 , 180};
  static const double angles_l1[2] = {47 , 0  };
  static const double angles_r1[2] = {47 , 180};
  static const double angles_l2[2] = {165, 0  };
  static const double angles_r2[2] = {165, 180};
}
#endif
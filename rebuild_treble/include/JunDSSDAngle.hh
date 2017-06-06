#ifndef JunDSSDAngle_HH
#define JunDSSDAngle_HH 1

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <cstdlib>

#include <TVector3.h>
#include <TMath.h>

#include "JunParMan.hh"
#include "JunErrors.hh"

using namespace std;

class JunDSSDAngle
{
private:
  JunDSSDAngle();
  static JunDSSDAngle* thisAngle;
public:
  virtual ~JunDSSDAngle();
  static JunDSSDAngle* Instance();
  double GetTheta(string dssdname,int i,int j);
  double GetPhi(string dssdname,int i,int j);
  double RealTheta(string dssdname,double x,double y);//*mm
  double RealPhi(string dssdname,double x,double y);//*mm
  int Check(string dssdname,int i,int j,double x,double y);
private:
  map<string,double> mapOfTheta;//*rad
  map<string,double> mapOfPhi;//*rad
};
#endif
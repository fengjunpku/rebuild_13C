#ifndef JunLossCorrection_HH
#define JunLossCorrection_HH

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <TMVA/TSpline2.h>

#include "JunErrors.hh"

using namespace std;
using namespace TMVA;


class JunLossCorrection
{
public:
  JunLossCorrection();
  virtual ~JunLossCorrection();
  void addDataFile(string filename,string discription);
  double correctEnergy(double range,double energy,string discription);
private:
  map<string,TSpline2*> mRvE;//range vs. energy
  map<string,TSpline2*> mEvR;//energy vs. range
};
#endif
#ifndef JunDataWriter_HH
#define JunDataWriter_HH

#include <iomanip>
#include <vector>
#include "stdio.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TString.h>

#include "reDefine.hh"
#include "JunParticle.hh"
#include "JunArrayOfParticle.hh"

using namespace std;
using namespace reDef;
//instance->openfile->gettree->recorde
class JunDataWriter
{
private:
  JunDataWriter();
public:
  virtual ~JunDataWriter();
  static JunDataWriter* Instance();
  void Fill();
  void Record();
  void Clear();
  void OpenFile(int runnum);
  
private:
  TTree *otree;
  TFile *ofile;
  static JunDataWriter* theWriter;
public:
  int num;// total num of he4 & be9
  int numHe4;
  int numBe9;// all Be9 on T0
  int numBeR;// recoil be9 on T0
  int numT1H;
  double t1;//time of particle 1
  double t2;
  double t3;
  JunArrayOfParticle ps;// array of particles
  JunParticle im;
  JunParticle qim;
  JunParticle mm;
  JunParticle mix;
  JunParticle mxo;
  JunParticle mxc;
  JunParticle q;
};
#endif
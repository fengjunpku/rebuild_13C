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

using namespace std;
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
  void OpenFile(int num);
  
private:
  TTree *otree;
  TFile *ofile;
  static JunDataWriter* theWriter;
public:
  int num;
  int numAlpha;
  int numBe9;
  JunParticle alpha;
  JunParticle breakup;
  JunParticle recoil;
  JunParticle im;
  JunParticle mm;
  JunParticle mix;
};
#endif
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
  void Clear();
  void OpenFile(int runnum);
  
private:
  TTree *otree;
  TFile *ofile;
  static JunDataWriter* theWriter;
public:
  int num;
  int numHe4;
  int numBe9;
  JunParticle he4;
  JunParticle be9;
  JunParticle be9b;//breakup
  JunParticle be9r;//recoil
  JunParticle im;
  JunParticle mm;
  JunParticle mix;
};
#endif
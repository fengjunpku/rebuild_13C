#ifndef JUNPIDMAN_HH
#define JUNPIDMAN_HH 1

#include <iostream>
#include <string>

#include <TCutG.h>
#include <TFile.h>
#include <TMath.h>

#include "JunParMan.hh"
#include "JunErrors.hh"

using namespace std;

class JunPIDMan
{
public:
  JunPIDMan();
  virtual ~JunPIDMan();
  void PrintPars();
  bool isHe4(string teleName,double de,double e);
  bool isBe(string teleName,double de,double e);
  int tellBe(string teleName,double de,double e,int i,int j);//0:false; 9:9Be; 10:10Be
  bool isRecoil(string teleName,double e,double theta);
  

private:
  void LoadAutoPars();
  void LoadCut();
  float calEf(double e,double de,float a,float b);
  TCutG *l0_Be,*l0_He4_f,*l0_He4_b;
  TCutG *r0_Be,*r0_He4_f,*r0_He4_b;
  TCutG *fRecoil;
  int _ni;
  int _nj;
  // 0: left; 1: right
  string _tnames[2];
  float a[2][16][16];
  float b[2][16][16];
  float cut[2][16][16];
  float rate[2][16][16];
  
};

#endif
#ifndef JUNTREEREADER_HH
#define JUNTREEREADER_HH 1

#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>

#include "reDefine.hh"
#include "JunErrors.hh"

using namespace std;

class JunTreeReader
{
public:
  TTree *ptree;
  
  //l0
  Double_t l0w1e[4];
  Double_t l0b7e[4];
  Double_t l0se;
  Int_t    l0hit;
  Int_t    l0i[4];
  Int_t    l0j[4];
  Int_t    l0wi[4];
  Int_t    l0wj[4];
  //r0
  Double_t r0w1e[4];
  Double_t r0b7e[4];
  Double_t r0se;
  Int_t    r0hit;
  Int_t    r0i[4];
  Int_t    r0j[4];
  Int_t    r0wi[4];
  Int_t    r0wj[4];
  
  JunTreeReader(int runnum);
  virtual ~JunTreeReader();
  Long64_t GetEntries();
  Int_t GetEntry(Long64_t iEntry);
  void Init();
  Int_t    GetInt(const string vname,const int p=0);
  Double_t GetDou(const string vname,const int p=0);

private:
  TFile *ifile;
};
#endif
#ifndef JUNTREEREADER_HH
#define JUNTREEREADER_HH 1

#include <iostream>
#include <string>

#include <TFile.h>
#include <TTree.h>

#include "reDefine.hh"
#include "JunErrors.hh"

using namespace std;
using namespace reDef;

class JunTreeReader
{
public:
  TTree *ptree;
  
  //l0
  Double_t l0w1e[4];
  Double_t l0b7e[4];
  Double_t l0se;
  Double_t l0t[4];
  Int_t    l0hit;
  Int_t    l0i[4];
  Int_t    l0j[4];
  Int_t    l0wi[4];
  Int_t    l0wj[4];
  Int_t    l0m[4];
  //r0
  Double_t r0w1e[4];
  Double_t r0b7e[4];
  Double_t r0se;
  Double_t r0t[4];
  Int_t    r0hit;
  Int_t    r0i[4];
  Int_t    r0j[4];
  Int_t    r0wi[4];
  Int_t    r0wj[4];
  Int_t    r0m[4];
  //l1
  Double_t l1w1e[4];
  Double_t l1se;
  Double_t l1t[4];
  Int_t    l1hit;
  Int_t    l1wi[4];
  Int_t    l1wj[4];
  Int_t    l1m[4];
  //r1
  Double_t r1w1e[4];
  Double_t r1se;
  Double_t r1t[4];
  Int_t    r1hit;
  Int_t    r1wi[4];
  Int_t    r1wj[4];
  Int_t    r1m[4];
  
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
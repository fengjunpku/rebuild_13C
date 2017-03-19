#ifndef JunParticle_HH
#define JunParticle_HH
#include <iostream>
#include <string>
#include <TROOT.h>
#include <TVector3.h>
#include <TMath.h>

using namespace std;
class JunParticle
{
public:
  JunParticle();
  JunParticle(string pName,Double_t pEnergy,TVector3 pDirection);
  JunParticle(string pName,Double_t pEnergy,Double_t pTheta,Double_t pPhi);
  virtual ~JunParticle();
  string name;
  TVector3 direction;
  Double_t energy;//*MeV
  Double_t theta;//*deg
  Double_t phi;//*deg
  ClassDef(JunParticle,1);
};
  ClassImp(JunParticle);
#endif
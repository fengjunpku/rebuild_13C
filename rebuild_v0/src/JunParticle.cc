#include "JunParticle.hh"


JunParticle::JunParticle()
{
  name      = "null";
  direction.SetXYZ(0,0,-1);
  theta     = 0;
  phi       = 0;
  energy    = -1000;
  time      = -99999;
}

JunParticle::JunParticle(string pName,Double_t pEnergy,TVector3 pDirection,Double_t pTime)
{
  name      = pName;
  direction = pDirection.Unit();
  energy    = pEnergy;
  theta     = direction.Theta()*TMath::RadToDeg();
  phi       = direction.Phi()*TMath::RadToDeg();
  time      = pTime;
}

JunParticle::JunParticle(string pName,Double_t pEnergy,Double_t pTheta,Double_t pPhi,Double_t pTime)
{
  name      = pName;
  energy    = pEnergy;
  direction.SetMagThetaPhi(1,pTheta,pPhi);
  theta     = direction.Theta()*TMath::RadToDeg();
  phi       = direction.Phi()*TMath::RadToDeg();
  time      = pTime;
}

JunParticle::~JunParticle()
{}
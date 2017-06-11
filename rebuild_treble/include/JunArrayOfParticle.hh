#ifndef JunArrayOfParticle_HH
#define JunArrayOfParticle_HH 1

#include "JunParticle.hh"

class JunArrayOfParticle
{
public:
  JunArrayOfParticle();
  virtual ~JunArrayOfParticle();
  JunParticle *ap;
  int GetNum(int Z,int A,int tele);
  void Add(JunParticle tp);
  JunParticle GetParticle(int Z,int A,int tele);
  int _num;
};

#endif
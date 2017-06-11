#include "JunArrayOfParticle.hh"

JunArrayOfParticle::JunArrayOfParticle()
{
  ap = new JunParticle[10];
  _num = 0;
}

int JunArrayOfParticle::GetNum(int Z,int A,int tele)
{
  int cnt = 0;
  for(int i=0;i<_num;i++)
  {
    if(ap[i].Z==Z && ap[i].A==A && ap[i].tflag==tele)
      cnt++;
  }
  return cnt;
}

JunParticle JunArrayOfParticle::GetParticle(int Z,int A,int tele)
{
  JunParticle stp;
  for(int i=0;i<_num;i++)
  {
    if(ap[i].Z==Z && ap[i].A==A && ap[i].tflag==tele)
      stp = ap[i];
  }
  return stp;
}

void JunArrayOfParticle::Add(JunParticle tp)
{
  ap[_num%10] = tp;
  _num++;
}

JunArrayOfParticle::~JunArrayOfParticle()
{}
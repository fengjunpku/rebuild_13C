#include "JunArrayOfParticle.hh"

JunArrayOfParticle::JunArrayOfParticle()
{
  ap = new JunParticle[10];
  _num = 0;
  _num_he4 = 0;
  _code = 0;
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

int JunArrayOfParticle::GetNum(int Z,int A)
{
  int cnt = 0;
  for(int i=0;i<_num;i++)
  {
    if(ap[i].Z==Z && ap[i].A==A)
      cnt++;
  }
  return cnt;
}

JunParticle *JunArrayOfParticle::GetParticle(int Z,int A,int tele)
{
  int numP = GetNum(Z,A,tele);
  if(0 == numP) return NULL;
  JunParticle *stp = new JunParticle[numP];
  int k = 0;
  for(int i=0;i<_num;i++)
  {
    if(ap[i].Z==Z && ap[i].A==A && ap[i].tflag==tele)
      stp[k++] = ap[i];
  }
  return stp;
}

JunParticle *JunArrayOfParticle::GetParticle(int Z,int A)
{
  int numP = GetNum(Z,A);
  if(0 == numP) return NULL;
  JunParticle *stp = new JunParticle[numP];
  int k = 0;
  for(int i=0;i<_num;i++)
  {
    if(ap[i].Z==Z && ap[i].A==A)
      stp[k++] = ap[i];
  }
  return stp;
}

void JunArrayOfParticle::Add(JunParticle tp)
{
  ap[_num%10] = tp;
  _num++;
  _code += tp.tflag;
  if(tp.Z == 2 && tp.A == 4) _num_he4++;
}

JunArrayOfParticle::~JunArrayOfParticle()
{}
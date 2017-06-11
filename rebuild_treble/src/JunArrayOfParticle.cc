#include "JunArrayOfParticle.hh"

JunArrayOfParticle::JunArrayOfParticle()
{
  ap = new JunParticle[10];
  _num = 0;
}

int JunArrayOfParticle::GetNum(int Z,int A,int tele)
{
  int cnt = 0;
  for(int i=0;i<10;i++)
  {
    if(ap[i].Z==Z && ap[i].A==A && ap[i].tflag==tele)
      cnt++;
  }
  return cnt;
}

void JunArrayOfParticle::Add(JunParticle tp)
{
  ap[_num%10] = tp;
  _num++;
  //cout<<_num<<endl;
}

JunArrayOfParticle::~JunArrayOfParticle()
{}
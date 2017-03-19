#include "JunPIDMan.hh"

JunPIDMan::JunPIDMan()
{
  _ni = 16;
  _nj = 16;
  _tnames[0] = "l0";
  _tnames[1] = "r0";
  LoadAutoPars();
  LoadCut();
}

JunPIDMan::~JunPIDMan()
{
  
}

void JunPIDMan::LoadAutoPars()
{
  for(int it=0;it<2;it++)
  {
    for(int i=0;i<_ni;i++)
    {
      for(int j=0;j<_nj;j++)
      {
        char filename[30];
        sprintf(filename,"pars/parACut_%s_%02d_%02d.txt",_tnames[it].c_str(),i,j);
        ifstream file(filename);
        if(!file || !file.is_open() || !file.good())
        {
          char err[60];
          sprintf(err,"Can not open file %s !",filename);
          MiaoError(err);
        }
        if(file.eof())
        {
          char err[60];
          sprintf(err,"Empty file: %s !",filename);
          MiaoError(err);
        }
        file>>a[it][i][j]>>b[it][i][j]>>cut[it][i][j]>>rate[it][i][j];
      }
    }
  }
}

void JunPIDMan::LoadCut()
{
  TFile *cutf;
  cutf = TFile::Open("cut/l0_be.root");
  cutf->GetObject("CUTG",l0_Be);
  cutf = TFile::Open("cut/l0he_f.root");
  cutf->GetObject("CUTG",l0_He4_f);
  cutf = TFile::Open("cut/l0he_b.root");
  cutf->GetObject("CUTG",l0_He4_b);
  cutf = TFile::Open("cut/r0_be.root");
  cutf->GetObject("CUTG",r0_Be);
  cutf = TFile::Open("cut/r0he_f.root");
  cutf->GetObject("CUTG",r0_He4_f);
  cutf = TFile::Open("cut/r0he_b.root");
  cutf->GetObject("CUTG",r0_He4_b);
  cutf = TFile::Open("cut/be9_front_recoil.root");
  cutf->GetObject("CUTG",fRecoil);
}

float JunPIDMan::calEf(double de,double e,float alpha,float beta)
{
  return TMath::Sqrt(e*de+alpha*de*de)+beta*e;
}

void JunPIDMan::PrintPars()
{
  printf("          |     a   |   b   |   cut  | rate  |\n");
  for(int it=0;it<2;it++)
  {
    for(int i=0;i<_ni;i++)
    {
      for(int j=0;j<_nj;j++)
      {
        printf(" %s %02d %02d : %5.5f %5.5f %5.5f %5.5f\n",_tnames[it].c_str(),i,j,a[it][i][j],b[it][i][j],cut[it][i][j],rate[it][i][j]);
      }
    }
  }
}

bool JunPIDMan::isBe(string teleName,double de,double e)
{
  if(teleName!="l0" && teleName!="r0")
    MiaoError("JunPIDMan::isBe() : teleName must be [l0/r0]");
  TCutG *theBe = NULL;
  if(teleName=="l0") theBe = l0_Be;
  if(teleName=="r0") theBe = r0_Be;
  return theBe->IsInside(e,de);
}

int JunPIDMan::tellBe(string teleName,double de,double e,int i,int j)
{
  if(i<0||j<0)
    return 0;
  if(!isBe(teleName,de,e)) 
    return 0;
  int sign;
  if(teleName=="l0") sign = 0;
  if(teleName=="r0") sign = 1;
  if(rate[sign][i][j]<2) 
    return 0;
  float ef = calEf(de,e,a[sign][i][j],b[sign][i][j]);
  if(ef<cut[sign][i][j]) 
    return 9;
  else 
    return 10;
}

bool JunPIDMan::isHe4(string teleName,double de,double e)
{
  if(teleName!="l0f" && teleName!="r0f" && teleName!="l0b" && teleName!="r0b")
    MiaoError("JunPIDMan::isHe4() : teleName must be [l0f/l0b/r0f/r0b]");
  TCutG *theHe =NULL;
  if(teleName=="l0f") theHe = l0_He4_f;
  if(teleName=="l0b") theHe = l0_He4_b;
  if(teleName=="r0f") theHe = r0_He4_f;
  if(teleName=="r0b") theHe = r0_He4_b;
  return theHe->IsInside(e,de);
}

bool JunPIDMan::isRecoil(string teleName,double e,double theta)//MeV * deg
{
  if(teleName != "front")
    MiaoError("JunPIDMan::isRecoil() : teleName should be [front/]");
  TCutG *cg = NULL;
  if(teleName == "front") cg = fRecoil;
  return cg->IsInside(theta,e);
}
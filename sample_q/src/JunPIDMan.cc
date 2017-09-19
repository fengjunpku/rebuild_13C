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
        char filename[50];
        sprintf(filename,"../rebuild_v0/pars/parACut_%s_%02d_%02d.txt",_tnames[it].c_str(),i,j);
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
  TFile *cutf = NULL;
  //l0
  cutf = TFile::Open("../rebuild_v0/cut/l0_be.root");  cutf->GetObject("CUTG",l0_Be);
  cutf = TFile::Open("../rebuild_v0/cut/l0he_w.root"); cutf->GetObject("CUTG",l0_He4_w);
  cutf = TFile::Open("../rebuild_v0/cut/l0he_f.root"); cutf->GetObject("CUTG",l0_He4_f);
  cutf = TFile::Open("../rebuild_v0/cut/l0he_b.root"); cutf->GetObject("CUTG",l0_He4_b);
  //r0
  cutf = TFile::Open("../rebuild_v0/cut/r0_be.root");  cutf->GetObject("CUTG",r0_Be);
  cutf = TFile::Open("../rebuild_v0/cut/r0he_w.root"); cutf->GetObject("CUTG",r0_He4_w);
  cutf = TFile::Open("../rebuild_v0/cut/r0he_f.root"); cutf->GetObject("CUTG",r0_He4_f);
  cutf = TFile::Open("../rebuild_v0/cut/r0he_b.root"); cutf->GetObject("CUTG",r0_He4_b);
  //l1
  cutf = TFile::Open("../rebuild_v0/cut/l1_he.root");  cutf->GetObject("CUTG",l1_He4);
  //r1
  cutf = TFile::Open("../rebuild_v0/cut/r1_he.root");  cutf->GetObject("CUTG",r1_He4);
  //MeV vs. Deg
  cutf = TFile::Open("../rebuild_v0/cut/be9_t0_recoil_20170813.root");
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
  if(e<=0 || de<=0) return false;
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
  if(teleName!="l0f" && teleName!="r0f" && teleName!="l0b" && teleName!="r0b" && teleName!="l0w" && teleName!="r0w" && teleName!="l1" && teleName!="r1")
    MiaoError("JunPIDMan::isHe4() : teleName must be [l0w/l0f/l0b/r0w/r0f/r0b/l1/r1]");
  if(e<=0 || de<=0) return false;
  TCutG *theHe = NULL;
  if(teleName=="l0w") theHe = l0_He4_w;//W1 vs BB7 + SSD
  if(teleName=="l0f") theHe = l0_He4_f;//W1 vs BB7
  if(teleName=="l0b") theHe = l0_He4_b;//BB7 vs SSD
  if(teleName=="r0w") theHe = r0_He4_w;
  if(teleName=="r0f") theHe = r0_He4_f;
  if(teleName=="r0b") theHe = r0_He4_b;
  if(teleName=="l1")  theHe = l1_He4;
  if(teleName=="r1")  theHe = r1_He4;
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
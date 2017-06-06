#include "JunTreeReader.hh"

JunTreeReader::JunTreeReader(int runnum)
{
  char infilename[30];
  sprintf(infilename,"sete%04d.root",runnum);
  TString infname(in_dir+infilename);
  ifile = TFile::Open(infname,"READ");
  if(!ifile || !ifile->IsOpen())
  {
    char err[30];
    sprintf(err," * JunTreeReader::JunTreeReader() :Can not open %s !",infname.Data());
    MiaoError(err);
  }
  ifile->GetObject("tree",ptree);
  Init();
}

JunTreeReader::~JunTreeReader()
{
  ifile->Close();
}

Long64_t JunTreeReader::GetEntries()
{
  if(!ptree) return 0;
  return ptree->GetEntriesFast();
}

Int_t JunTreeReader::GetEntry(Long64_t iEntry)
{
  if(!ptree) return 0;
  return ptree->GetEntry(iEntry);
}

void JunTreeReader::Init()
{
  if(!ptree) MiaoError(" * JunTreeReader::Init() : ptree is NULL !");
  //l0
  ptree->SetBranchAddress("l0w1e", l0w1e);
  ptree->SetBranchAddress("l0b7e", l0b7e);
  ptree->SetBranchAddress("l0se", &l0se);
  ptree->SetBranchAddress("l0hit",&l0hit);
  ptree->SetBranchAddress("l0i",   l0i);
  ptree->SetBranchAddress("l0j",   l0j);
  ptree->SetBranchAddress("l0wi",  l0wi);
  ptree->SetBranchAddress("l0wj",  l0wj);
  ptree->SetBranchAddress("l0t",   l0t);
  ptree->SetBranchAddress("l0m",   l0m);
  //r0
  ptree->SetBranchAddress("r0w1e", r0w1e);
  ptree->SetBranchAddress("r0b7e", r0b7e);
  ptree->SetBranchAddress("r0se", &r0se);
  ptree->SetBranchAddress("r0hit",&r0hit);
  ptree->SetBranchAddress("r0i",   r0i);
  ptree->SetBranchAddress("r0j",   r0j);
  ptree->SetBranchAddress("r0wi",  r0wi);
  ptree->SetBranchAddress("r0wj",  r0wj);
  ptree->SetBranchAddress("l0t",   r0t);
  ptree->SetBranchAddress("l0m",   r0m);
  //l1
  ptree->SetBranchAddress("l1w1e", l1w1e);
  ptree->SetBranchAddress("l1se", &l1se);
  ptree->SetBranchAddress("l1hit",&l1hit);
  ptree->SetBranchAddress("l1wi",  l1wi);
  ptree->SetBranchAddress("l1wj",  l1wj);
  ptree->SetBranchAddress("l1t",   l1t);
  ptree->SetBranchAddress("l1m",   l1m);
  //r1
  ptree->SetBranchAddress("r1w1e", r1w1e);
  ptree->SetBranchAddress("r1se", &r1se);
  ptree->SetBranchAddress("r1hit",&r1hit);
  ptree->SetBranchAddress("r1wi",  r1wi);
  ptree->SetBranchAddress("r1wj",  r1wj);
  ptree->SetBranchAddress("r1t",   r1t);
  ptree->SetBranchAddress("r1m",   r1m);
}

Int_t JunTreeReader::GetInt(const string vname,const int p)
{
  if(p<0 || p>3) MiaoError("JunTreeReader::GetInt() : p should in range [0,3] ! ");
  //l0
  if(vname == "l0hit") return l0hit;
  if(vname == "l0i")   return l0i[p];
  if(vname == "l0j")   return l0j[p];
  if(vname == "l0wi")  return l0wi[p];
  if(vname == "l0wj")  return l0wj[p];
  if(vname == "l0m")   return l0m[p];
  //r0
  if(vname == "r0hit") return r0hit;
  if(vname == "r0i")   return r0i[p];
  if(vname == "r0j")   return r0j[p];
  if(vname == "r0wi")  return r0wi[p];
  if(vname == "r0wj")  return r0wj[p];
  if(vname == "r0m")   return r0m[p];
  //l1
  if(vname == "l1hit") return l1hit;
  if(vname == "l1wi")  return l1wi[p];
  if(vname == "l1wj")  return l1wj[p];
  if(vname == "l1m")   return l1m[p];
  //r1
  if(vname == "r1hit") return r1hit;
  if(vname == "r1wi")  return r1wi[p];
  if(vname == "r1wj")  return r1wj[p];
  if(vname == "r1m")   return r1m[p];
  //last
  char err[30];
  sprintf(err,"JunTreeReader::GetInt() : %s is illegal vname !",vname.c_str());
  MiaoError(err);
  return 0;
}

Double_t JunTreeReader::GetDou(const string vname,const int p)
{
  if(p<0 || p>3) MiaoError("JunTreeReader::GetDou() : p should in range [0,3] ! ");
  //l0
  if(vname == "l0se")  return l0se;
  if(vname == "l0w1e") return l0w1e[p];
  if(vname == "l0b7e") return l0b7e[p];
  if(vname == "l0t")   return l0t[p];
  //r0
  if(vname == "r0se")  return r0se;
  if(vname == "r0w1e") return r0w1e[p];
  if(vname == "r0b7e") return r0b7e[p];
  if(vname == "r0t")   return r0t[p];
  //l1
  if(vname == "l1se")  return l1se;
  if(vname == "l1w1e") return l1w1e[p];
  if(vname == "l1t")   return l1t[p];
  //r1
  if(vname == "r1se")  return r1se;
  if(vname == "r1w1e") return r1w1e[p];
  if(vname == "r1t")   return r1t[p];
  //last
  char err[30];
  sprintf(err,"JunTreeReader::GetDou() : %s is illegal vname !",vname.c_str());
  MiaoError(err);
  return 0;
}
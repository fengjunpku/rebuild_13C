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
  //r0
  ptree->SetBranchAddress("r0w1e", r0w1e);
  ptree->SetBranchAddress("r0b7e", r0b7e);
  ptree->SetBranchAddress("r0se", &r0se);
  ptree->SetBranchAddress("r0hit",&r0hit);
  ptree->SetBranchAddress("r0i",   r0i);
  ptree->SetBranchAddress("r0j",   r0j);
  ptree->SetBranchAddress("r0wi",  r0wi);
  ptree->SetBranchAddress("r0wj",  r0wj);
  }
#include "JunDataWriter.hh"

JunDataWriter* JunDataWriter::theWriter = NULL;

JunDataWriter::JunDataWriter()
{}

JunDataWriter::~JunDataWriter()
{
  delete ofile;
  //delete otree;//may cause problem
  printf("End of class JunDataWriter\n");
}

////////////////////////////////////
void JunDataWriter::OpenFile(int runnum)
{
  char opfile[20];
  sprintf(opfile,"front%04d.root",runnum);
  TString filename(ou_dir+opfile);
  ofile = new TFile(filename,"RECREATE");
  otree = new TTree("tree","tree of particles");
  otree->Branch("num",    &num,    "num/I");
  otree->Branch("numHe4", &numHe4, "numHe4/I");
  otree->Branch("numBe9", &numBe9, "numBe9/I");
  otree->Branch("numT1H", &numT1H, "numT1H/I");
  otree->Branch("he4",    &he4,  32000,3);
  otree->Branch("he4t0",  &he4t0,32000,3);
  otree->Branch("he4t1",  &he4t1,32000,3);
  otree->Branch("be9",    &be9,  32000,3);
  otree->Branch("be9b",   &be9b, 32000,3);
  otree->Branch("be9r",   &be9r, 32000,3);
  otree->Branch("t1h",    &t1h,  32000,3);
  otree->Branch("im",     &im,   32000,3);
  otree->Branch("mm",     &mm,   32000,3);
  otree->Branch("mix",    &mix,  32000,3);
  otree->Branch("qim",    &qim,  32000,3);
  otree->Branch("q",      &q,    32000,3);
}
//////////////////////////
JunDataWriter* JunDataWriter::Instance()
{
  if(theWriter==NULL)
    theWriter = new JunDataWriter();
  return theWriter;
}
/////////////////////////
void JunDataWriter::Record()
{
  ofile->cd();
  otree->Write();
  ofile->Close();
}

void JunDataWriter::Fill()
{
  ofile->cd();
  otree->Fill();
}

void JunDataWriter::Clear()
{
  JunParticle p;
  num     = 0;
  numHe4  = 0;
  numBe9  = 0;
  he4  = p;
  he4t0= p;
  he4t1= p;
  be9  = p;
  be9b = p;
  be9r = p;
  im   = p;
  mm   = p;
  mix  = p;
  qim  = p;
  q    = p;
  t1h  = p;
}
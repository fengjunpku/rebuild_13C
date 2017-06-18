#include "JunDataWriter.hh"

JunDataWriter* JunDataWriter::theWriter = NULL;

JunDataWriter::JunDataWriter()
{}

JunDataWriter::~JunDataWriter()
{
  delete ofile;
  delete jg;
  delete jh;
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
  otree->Branch("numBeR", &numBeR, "numBeR/I");
  otree->Branch("numT1H", &numT1H, "numT1H/I");
  otree->Branch("ps",     &ps,   32000,3);
  otree->Branch("im",     &im,   32000,3);
  otree->Branch("mm",     &mm,   32000,3);
  otree->Branch("mix",    &mix,  32000,3);
  otree->Branch("qim",    &qim,  32000,3);
  otree->Branch("q",      &q,    32000,3);
  jg = new TGraph();
  jg->SetNameTitle("g","g");
  jh = new TH2F("h","h",250,10,35,250,10,35);
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
  jg->Write();
  jh->Write();
  ofile->Close();
}

void JunDataWriter::Fill()
{
  ofile->cd();
  otree->Fill();
}

void JunDataWriter::Clear()
{
  num     = 0;
  numHe4  = 0;
  numBe9  = 0;
  numBeR  = 0;
  JunParticle p;
  im   = p;
  mm   = p;
  mix  = p;
  qim  = p;
  q    = p;
  JunArrayOfParticle _ps;
  ps   = _ps;
}
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
  otree->Branch("numBeR", &numBeR, "numBeR/I");
  otree->Branch("numT1H", &numT1H, "numT1H/I");
  otree->Branch("numT0H", &numT0H, "numT0H/I");
  otree->Branch("t1",    &t1,    "t1/D");
  otree->Branch("t2",    &t2,    "t2/D");
  otree->Branch("t3",    &t3,    "t3/D");
  otree->Branch("trm", trm,"trm[12]/I");
  otree->Branch("trt", trt,"trt[12]/D");
  otree->Branch("ps",     &ps,   32000,3);
  otree->Branch("im",     &im,   32000,3);
  otree->Branch("mm",     &mm,   32000,3);
  otree->Branch("mix",    &mix,  32000,3);
  otree->Branch("mxo",    &mxo,  32000,3);
  otree->Branch("mxc",    &mxc,  32000,3);
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
  num     = 0;
  numHe4  = 0;
  numBe9  = 0;
  numBeR  = 0;
  numT1H  = 0;
  numT0H  = 0;
  t1 = TMath::QuietNaN();
  t2 = TMath::QuietNaN();
  t3 = TMath::QuietNaN();
  JunParticle p;
  im   = p;
  mm   = p;
  mix  = p;
  mxo  = p;
  mxc  = p;
  qim  = p;
  q    = p;
  JunArrayOfParticle _ps;
  ps   = _ps;
  for(int i=0;i<12;i++)
  {
    trm[i] = 0;
    trt[i] = TMath::QuietNaN();
  }
}
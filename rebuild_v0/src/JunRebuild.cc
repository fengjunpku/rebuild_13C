#include "JunRebuild.hh"

JunRebuild::JunRebuild(int runnum)
{
  runno = runnum;
  //Load Setup
  JunParMan::Instance()->ReadParFile("setup_13C.par");
  //Init Angle
  pAngle = JunDSSDAngle::Instance();
  //Load EnergyLoss Correction
  ploss = new JunLossCorrection();
  ploss->addDataFile("He4_in_Al.txt","He4InAl");
  ploss->addDataFile("Be9_in_Al.txt","Be9InAl");
  //Load PID Cut
  pid = new JunPIDMan();
  //reader
  pread = new JunTreeReader(runnum);
  //writer
  pwrite = JunDataWriter::Instance();
  pwrite->OpenFile(runnum);
}

JunRebuild::~JunRebuild()
{
}

void JunRebuild::Loop()
{
  long numOfEntries = pread->GetEntries();
  long onePC = numOfEntries>100?long(numOfEntries/100):1;
  printf(" # rebuild sete%04d #\n",runno);
  for(long ie=0;ie<numOfEntries;ie++)
  {
    if(ie%onePC == 0)
    {
      printf("\r --> Process sete%04d, Events: %ld/%ld %6.1f %%",runno,ie,numOfEntries,float(ie/onePC));
      fflush(stdout);
    }
    Reset();
    pread->GetEntry(ie);
    anaT0("l0");
    anaT0("r0");
    anaT1("l1");
    anaT1("r1");
    reIM();
    reMM();
    Mix();
    numTotal = numOfBe9 + numOfHe4 + numOfT1H;
    if(numTotal>0) Fill();
  }
}

void JunRebuild::Reset()
{
  pwrite->Clear();
  numTotal  = 0;
  numOfHe4  = 0;
  numOfBe9  = 0;
  numOfT1H  = 0;
  nRecoiBe9 = 0;
  nBreakBe9 = 0;
}

void JunRebuild::Save()
{
  pwrite->Record();
}

void JunRebuild::Fill()
{
  pwrite->num = numTotal;
  pwrite->numHe4 = numOfHe4;
  pwrite->numBe9 = numOfBe9;
  pwrite->numT1H = numOfT1H;
  pwrite->Fill();
}

void JunRebuild::anaT0(const string tname)
{
  if(tname != "l0" && tname != "r0")
    MiaoError("JunRebuild::anaT0() : tname should be l0/r0 !");
  //vars
  int    t0hit = pread->GetInt(tname+"hit");
  double t0se  = pread->GetDou(tname+"se");
  //loop n hit
  bool match_e3 = t0se>0?false:true;
  for(int it=0;it<t0hit;it++)
  {
    int    t0i   = pread->GetInt(tname+"i",  it);
    int    t0j   = pread->GetInt(tname+"j",  it);
    int    t0wi  = pread->GetInt(tname+"wi", it);
    int    t0wj  = pread->GetInt(tname+"wj", it);
    double t0w1e = pread->GetDou(tname+"w1e",it);
    double t0b7e = pread->GetDou(tname+"b7e",it);
    double e[3] = {t0w1e,t0b7e,t0se};
    int ij[2] = {t0i,t0j};
    int wij[2] = {t0wi,t0wj};
    numOfHe4 += nT0He4(tname,e,ij,match_e3);
    numOfBe9 += nT0Be9(tname,e,ij,wij);
  }
}

void JunRebuild::anaT1(const string tname)
{
  if(tname != "l1" && tname != "r1")
    MiaoError("JunRebuild::anaT1() : tname should be l1/r1 !");
  //vars
  int    t1hit = pread->GetInt(tname+"hit");
  double t1se  = pread->GetDou(tname+"se");
  //loop n hit
  bool match_e2 = t1se>0?false:true;
  for(int it=0;it<t1hit;it++)
  {
    int    t1wi  = pread->GetInt(tname+"wi", it);
    int    t1wj  = pread->GetInt(tname+"wj", it);
    double t1w1e = pread->GetDou(tname+"w1e",it);
    double e[2] = {t1w1e,t1se};
    int wij[2] = {t1wi,t1wj};
    numOfHe4 += nT1He4(tname,e,wij,match_e2);
    numOfT1H += nT1More(tname,e,wij);
  }
}

int JunRebuild::nT0He4(const string tname,double *e,int *ij,bool &matchSSD)
{
  if(ij[0]<0 || ij[1]<0)
    return 0;
  const double *dl = NULL;
  if("l0" == tname) dl = DL_l0;
  if("r0" == tname) dl = DL_r0;
  int nhe4 = 0;
  //energy in 3 detectors
  if(!matchSSD && pid->isHe4(tname+"w",e[0],e[1]+e[2]) && pid->isHe4(tname+"b",e[1],e[2]))
  {
    double et = ploss->GetE(dl,e,3,"He4InAl");
    //double et = e[0]+e[1]+e[2];
    double th = pAngle->GetTheta(tname+"bb7",ij[0],ij[1]);
    double ph =   pAngle->GetPhi(tname+"bb7",ij[0],ij[1]);
    JunParticle theAlpha("alpha",et,th,ph);
    pwrite->he4 = theAlpha;
    matchSSD = true;
    nhe4++;
  }
  //energy in 2 detectors
  if(pid->isHe4(tname+"f",e[0],e[1]))
  {
    double et = ploss->GetE(dl,e,2,"He4InAl");
    //double et = e[0]+e[1];
    double th = pAngle->GetTheta(tname+"bb7",ij[0],ij[1]);
    double ph =   pAngle->GetPhi(tname+"bb7",ij[0],ij[1]);
    JunParticle theAlpha("alpha",et,th,ph);
    pwrite->he4 = theAlpha;
    nhe4++;
  }
  return nhe4;
}

int JunRebuild::nT0Be9(const string tname,double *e,int *ij,int *wij)
{
  if(ij[0]<0 || ij[1]<0)
    return 0;
  const double *dl = NULL;
  if("l0" == tname) dl = DL_l0;
  if("r0" == tname) dl = DL_r0;
  int nbe9 = 0;
  //
  int be_flag = pid->tellBe(tname,e[0],e[1],wij[0],wij[1]);
  if(be_flag == 9)
  {
    double et = ploss->GetE(dl,e,2,"Be9InAl");
    //double et = e[0]+e[1];
    double th = pAngle->GetTheta(tname+"bb7",ij[0],ij[1]);
    double ph =   pAngle->GetPhi(tname+"bb7",ij[0],ij[1]);
    JunParticle theBe9("break",et,th,ph);
    pwrite->be9 = theBe9;
    nbe9++;
    //tell recoil or break
    if(pid->isRecoil("front",et,th*TMath::RadToDeg()))
    {
      pwrite->be9r = theBe9;
      nRecoiBe9++;
    }
    else
    {
      pwrite->be9b = theBe9;
      nBreakBe9++;
    }
  }
  return nbe9;
}

int JunRebuild::nT1He4(const string tname,double *e,int *wij,bool &matchSSD)
{
  if(wij[0]<0 || wij[1]<0)
    return 0;
  const double *dl = NULL;
  if("l1" == tname) dl = DL_l1;
  if("r1" == tname) dl = DL_r1;
  int nhe4 = 0;
  //energy in two detectors
  if(!matchSSD && pid->isHe4(tname,e[0],e[1]))
  {
    double et = ploss->GetE(dl,e,2,"He4InAl");
    //double et = e[0]+e[1];
    double th = pAngle->GetTheta(tname+"w1",wij[0],wij[1]);
    double ph =   pAngle->GetPhi(tname+"w1",wij[0],wij[1]);
    JunParticle theAlpha("alpha",et,th,ph);
    pwrite->he4 = theAlpha;
    nhe4++;
  }
  return nhe4;
}

int JunRebuild::nT1More(const string tname,double *e,int *wij)
{
  if(wij[0]<0 || wij[1]<0)
    return 0;
  const double *dl = NULL;
  if("l1" == tname) dl = DL_l1;
  if("r1" == tname) dl = DL_r1;
  int nt1h = 0;
  //only in W1
  if(e[1]<=0 && e[0]>7.9)
  {
    double et = ploss->GetE(dl,e,1,"Be9InAl");
    //double et = e[0];
    double th = pAngle->GetTheta(tname+"w1",wij[0],wij[1]);
    double ph =   pAngle->GetPhi(tname+"w1",wij[0],wij[1]);
    JunParticle theT1H("alpha",et,th,ph);
    pwrite->t1h = theT1H;
    nt1h++;
  }
  return nt1h;
}

void JunRebuild::reIM()
{
  if(nBreakBe9>0 && 1 == numOfHe4)
  {
    double ep1 = pwrite->he4.energy;
    double ep2 = pwrite->be9b.energy;
    TVector3 dir1 = TMath::Sqrt(2*Mass_He4*ep1)*(pwrite->he4.direction);
    TVector3 dir2 = TMath::Sqrt(2*Mass_Be9*ep2)*(pwrite->be9b.direction);
    TVector3 dir_recon = dir1 + dir2;
    double ene_recon = ep1 + ep2 - dir_recon*dir_recon/Mass_C13/2.;
    JunParticle IM("im",ene_recon,dir_recon);
    pwrite->im = IM;
    //qim
    TVector3 dir0 = TMath::Sqrt(2*65*Mass_C13)*TVector3(0,0,1);
    TVector3 dir3 = dir0 - dir1 -dir2;
    double ep3 = dir3*dir3/2./Mass_Be9;
    JunParticle QIM("qim",ep1+ep2+ep3-65,TVector3(0,0,1));
    pwrite->qim = QIM;
  }
}

void JunRebuild::reMM()
{
  if(nRecoiBe9>0)
  {
    double bEn = 65;//*MeV
    double epr = pwrite->be9r.energy;
    TVector3 dirR = TMath::Sqrt(2*Mass_Be9*epr)*(pwrite->be9r.direction);
    TVector3 dir0(0,0,1);
    dir0 = TMath::Sqrt(2*Mass_C13*bEn)*dir0;
    TVector3 dir_recon = dir0 - dirR;
    double ene_recon = bEn - epr - dir_recon*dir_recon/Mass_C13/2.;
    JunParticle MM("mm",ene_recon,dir_recon);
    pwrite->mm = MM;
  }
  if(nRecoiBe9==0 && numOfT1H>0)
  {
    double bEn = 65;//*MeV
    double epr = pwrite->t1h.energy;
    TVector3 dirR = TMath::Sqrt(2*Mass_Be9*epr)*(pwrite->t1h.direction);
    TVector3 dir0(0,0,1);
    dir0 = TMath::Sqrt(2*Mass_C13*bEn)*dir0;
    TVector3 dir_recon = dir0 - dirR;
    double ene_recon = bEn - epr - dir_recon*dir_recon/Mass_C13/2.;
    JunParticle MM("mm",ene_recon,dir_recon);
    pwrite->mm = MM;
  }
}

void JunRebuild::Mix()
{
  if(nBreakBe9>0 && 1 == numOfHe4 && 1 == numOfT1H)
  {
    if(lastBe.energy>0)
    {
      double ep1 = pwrite->he4.energy;
      double ep2 = lastBe.energy;
      TVector3 dir1 = TMath::Sqrt(2*Mass_He4*ep1)*(pwrite->he4.direction);
      TVector3 dir2 = TMath::Sqrt(2*Mass_Be9*ep2)*(lastBe.direction);
      TVector3 dir_recon = dir1 + dir2;
      double ene_recon = ep1 + ep2 - dir_recon*dir_recon/Mass_C13/2.;
      JunParticle MIX("mix",ene_recon,dir_recon);
      pwrite->mix = MIX;
    }
    lastBe = pwrite->be9b;
  }
}
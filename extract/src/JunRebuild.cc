#include "JunRebuild.hh"

JunRebuild::JunRebuild(int runnum)
{
  runno = runnum;
  //Load Setup
  JunParMan::Instance()->ReadParFile("../rebuild_v0/setup_13C.par");
  //Init Angle
  pAngle = JunDSSDAngle::Instance();
  //Load EnergyLoss Correction
  ploss = new JunLossCorrection();
  ploss->addDataFile("He4_in_Al.txt","He4InAl");
  ploss->addDataFile("Be9_in_Al.txt","Be9InAl");
  ploss->addDataFile("He4_in_Be9.txt","He4InBe");
  ploss->addDataFile("Be9_in_Be9.txt","Be9InBe");
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
  //long onePC = numOfEntries>100?long(numOfEntries/100):1;
  printf(" # rebuild sete%04d #\n",runno);
  for(long ie=0;ie<numOfEntries;ie++)
  {
    //if(ie%onePC == 0)
    //{
      //printf("\r --> Process sete%04d, Events: %ld/%ld %6.1f %%",runno,ie,numOfEntries,float(ie/onePC));
      //fflush(stdout);
    //}
    Reset();
    pread->GetEntry(ie);
    getTriggerInfo();
    anaT0("l0");
    anaT0("r0");
    //anaT1("l1");
    //anaT1("r1");
    numTotal = numOfBe9 + numOfHe4 ;
    if(numTotal>0) Fill();
  }
}

void JunRebuild::getTriggerInfo()
{
  pwrite->trm[0]  = pread->gl0m;
  pwrite->trm[1]  = pread->gr0m;
  pwrite->trm[2]  = pread->gl2s;
  pwrite->trm[3]  = pread->gr2s;
  pwrite->trm[4]  = pread->gfms;
  pwrite->trm[5]  = pread->gfmss;
  pwrite->trm[6]  = pread->gl0s;
  pwrite->trm[7]  = pread->gl1s;
  pwrite->trm[8]  = pread->gr0s;
  pwrite->trm[9]  = pread->gr1s;
  pwrite->trm[10] = pread->gssd;
  pwrite->trm[11] = pread->gfmm;
  //
  pwrite->trt[0]  = pread->gl0mt;
  pwrite->trt[1]  = pread->gr0mt;
  pwrite->trt[2]  = pread->gl2st;
  pwrite->trt[3]  = pread->gr2st;
  pwrite->trt[4]  = pread->gfmst;
  pwrite->trt[5]  = pread->gfmsst;
  pwrite->trt[6]  = pread->gl0st;
  pwrite->trt[7]  = pread->gl1st;
  pwrite->trt[8]  = pread->gr0st;
  pwrite->trt[9]  = pread->gr1st;
  pwrite->trt[10] = pread->gssdt;
  pwrite->trt[11] = pread->gfmmt;
}

void JunRebuild::Reset()
{
  pwrite->Clear();
  numTotal  = 0;
  numOfHe4  = 0;
  numOfBe9  = 0;
  numOfT0H  = 0;
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
  pwrite->numBeR = nRecoiBe9;
  pwrite->numT1H = numOfT1H;
  pwrite->numT0H = numOfT0H;
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
    double t0t   = pread->GetDou(tname+"t",  it);
    double e[3] = {t0w1e,t0b7e,t0se};
    int ij[2] = {t0i,t0j};
    int wij[2] = {t0wi,t0wj};
    numOfHe4 += nT0He4(tname,e,wij,ij,t0t,match_e3);
    numOfBe9 += nT0Be9(tname,e,wij,ij,t0t);
    //numOfT0H += nT0More(tname,e,wij,t0t);
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
    double t1t   = pread->GetDou(tname+"t",  it);
    double e[2] = {t1w1e,t1se};
    int wij[2] = {t1wi,t1wj};
    numOfHe4 += nT1He4(tname,e,wij,t1t,match_e2);
    numOfT1H += nT1More(tname,e,wij,t1t);
  }
}

int JunRebuild::nT0He4(const string tname,double *e,int *wij,int *ij,double time,bool &matchSSD)
{
  if(ij[0]<0 || ij[1]<0)
    return 0;
  const double *dl = NULL;
  if("l0" == tname) dl = DL_l0;
  if("r0" == tname) dl = DL_r0;
  int nhe4 = 0;
  int telecode = JunParMan::Instance()->GetPar(tname+"code");
  //
  double th = pAngle->GetTheta(tname,wij[0],wij[1],ij[0],ij[1]);
  double ph =   pAngle->GetPhi(tname,wij[0],wij[1],ij[0],ij[1]);
  if(TMath::IsNaN(th)) return 0;
  //double th = pAngle->GetTheta(tname+"bb7",ij[0],ij[1]);
  //double ph =   pAngle->GetPhi(tname+"bb7",ij[0],ij[1]);
  double angle = ploss->calAngle(th,ph,tname);
  //energy in 3 detectors
  if(!matchSSD && pid->isHe4(tname+"w",e[0],e[1]+e[2]) && pid->isHe4(tname+"b",e[1],e[2]))
  {
    double et = ploss->GetE(dl,e,3,"He4InAl",angle);//dead layer loss
    et = ploss->correctEnergy(2*halfTT/TMath::Cos(th),et,"He4InBe");//target loss
    JunParticle theAlpha("alpha",et,th,ph,time,4,2,telecode);
    theAlpha.SetNote("t0ssd");
    for(int i=0;i<3;i++) {theAlpha.des[i] = e[i];}
    pwrite->ps.Add(theAlpha);
    matchSSD = true;
    nhe4++;
  }
  //energy in 2 detectors
  if(pid->isHe4(tname+"f",e[0],e[1]))
  {
    //double et = e[0]+e[1];
    double et = ploss->GetE(dl,e,2,"He4InAl",angle);//dead layer loss
    et = ploss->correctEnergy(2*halfTT/TMath::Cos(th),et,"He4InBe");//target loss
    JunParticle theAlpha("alpha",et,th,ph,time,4,2,telecode);
    theAlpha.SetNote("t0bb7");
    for(int i=0;i<2;i++) {theAlpha.des[i] = e[i];}
    pwrite->ps.Add(theAlpha);
    nhe4++;
  }
  return nhe4;
}

int JunRebuild::nT0Be9(const string tname,double *e,int *wij,int *ij,double time)
{
  if(ij[0]<0 || ij[1]<0)
    return 0;
  const double *dl = NULL;
  if("l0" == tname) dl = DL_l0;
  if("r0" == tname) dl = DL_r0;
  int nbe9 = 0;
  int telecode = JunParMan::Instance()->GetPar(tname+"code");
  //
  int be_flag = pid->tellBe(tname,e[0],e[1],wij[0],wij[1]);
  if(be_flag == 9)
  {
    double th = pAngle->GetTheta(tname,wij[0],wij[1],ij[0],ij[1]);
    double ph =   pAngle->GetPhi(tname,wij[0],wij[1],ij[0],ij[1]);
    //double th = pAngle->GetTheta(tname+"bb7",ij[0],ij[1]);
    //double ph =   pAngle->GetPhi(tname+"bb7",ij[0],ij[1]);
    if(TMath::IsNaN(th)) return 0;
    double angle = ploss->calAngle(th,ph,tname);
    //energy
    //double et = e[0]+e[1];
    double et = ploss->GetE(dl,e,2,"Be9InAl",angle);//dead layer loss
    et = ploss->correctEnergy(2*halfTT/TMath::Cos(th),et,"Be9InBe");//target loss
    JunParticle theBe9("be9",et,th,ph,time,9,4,telecode);
    nbe9++;
    //tell recoil or break
    if(pid->isRecoil("front",et,th*TMath::RadToDeg()))
    {
      theBe9.SetNote("t0recoil");
      nRecoiBe9++;
    }
    else
    {
      theBe9.SetNote("t0break");
      nBreakBe9++;
    }
    for(int i=0;i<2;i++) {theBe9.des[i] = e[i];}
    pwrite->ps.Add(theBe9);
  }
  return nbe9;
}

int JunRebuild::nT0More(const string tname,double *e,int *wij,double time)
{
  if(wij[0]<0 || wij[1]<0)
    return 0;
  const double *dl = NULL;
  if("l0" == tname) dl = DL_l0;
  if("r0" == tname) dl = DL_r0;
  int nt0h = 0;
  int telecode = JunParMan::Instance()->GetPar(tname+"code");
  //
  if(e[1]<=0 && e[0]>0.5 && e[0]<20)//>= 20MeVnot be9
  {
    double th = pAngle->GetTheta(tname+"w1",wij[0],wij[1]);
    double ph =   pAngle->GetPhi(tname+"w1",wij[0],wij[1]);
    double angle = ploss->calAngle(th,ph,tname);
    //energy
    //double et = e[0];
    double et = ploss->GetE(dl,e,1,"Be9InAl",angle);//dead layer loss
    et = ploss->correctEnergy(2*halfTT/TMath::Cos(th),et,"Be9InBe");//target loss
    JunParticle theT0H("t0h",et,th,ph,time,9,4,telecode);
    theT0H.SetNote("t0more");
    theT0H.des[0] = e[0];
    pwrite->ps.Add(theT0H);
    nt0h++;
  }
  return nt0h;
}

int JunRebuild::nT1He4(const string tname,double *e,int *wij,double time,bool &matchSSD)
{
  if(wij[0]<0 || wij[1]<0)
    return 0;
  const double *dl = NULL;
  if("l1" == tname) dl = DL_l1;
  if("r1" == tname) dl = DL_r1;
  int nhe4 = 0;
  int telecode = JunParMan::Instance()->GetPar(tname+"code");
  //energy in two detectors
  if(!matchSSD && pid->isHe4(tname,e[0],e[1]))
  {
    double th = pAngle->GetTheta(tname+"w1",wij[0],wij[1]);
    double ph =   pAngle->GetPhi(tname+"w1",wij[0],wij[1]);
    double angle = ploss->calAngle(th,ph,tname);
    //energy
    //double et = e[0]+e[1];
    double et = ploss->GetE(dl,e,2,"He4InAl",angle);//dead layer loss
    et = ploss->correctEnergy(2*halfTT/TMath::Cos(th),et,"He4InBe");//target loss
    JunParticle theAlpha("alpha",et,th,ph,time,4,2,telecode);
    theAlpha.SetNote("t1ssd");
    pwrite->ps.Add(theAlpha);
    nhe4++;
  }
  return nhe4;
}

int JunRebuild::nT1More(const string tname,double *e,int *wij,double time)
{
  if(wij[0]<0 || wij[1]<0)
    return 0;
  const double *dl = NULL;
  if("l1" == tname) dl = DL_l1;
  if("r1" == tname) dl = DL_r1;
  int nt1h = 0;
  int telecode = JunParMan::Instance()->GetPar(tname+"code");
  //only in W1
  if(e[1]<=0 && e[0]>7.9)
  {
    double th = pAngle->GetTheta(tname+"w1",wij[0],wij[1]);
    double ph =   pAngle->GetPhi(tname+"w1",wij[0],wij[1]);
    double angle = ploss->calAngle(th,ph,tname);
    //energy
    //double et = e[0];
    double et = ploss->GetE(dl,e,1,"Be9InAl",angle);//dead layer loss
    et = ploss->correctEnergy(2*halfTT/TMath::Cos(th),et,"Be9InBe");//target loss
    JunParticle theT1H("t1h",et,th,ph,time,9,4,telecode);
    theT1H.SetNote("t1more");
    pwrite->ps.Add(theT1H);
    nt1h++;
  }
  return nt1h;
}

JunParticle JunRebuild::getIM(JunParticle break_he,JunParticle break_be)
{
  double ep1 = break_he.energy;
  double ep2 = break_be.energy;
  TVector3 dir1 = TMath::Sqrt(2*Mass_He4*ep1)*(break_he.direction);
  TVector3 dir2 = TMath::Sqrt(2*Mass_Be9*ep2)*(break_be.direction);
  TVector3 dir_recon = dir1 + dir2;
  double ene_recon = ep1 + ep2 - dir_recon*dir_recon/Mass_C13/2.;
  JunParticle IM("im",ene_recon,dir_recon);
  return IM;
}

JunParticle JunRebuild::getMM(JunParticle recoil)
{
  double bEn = 65;//*MeV
  double epr = recoil.energy;
  TVector3 dirR = TMath::Sqrt(2*Mass_Be9*epr)*(recoil.direction);
  TVector3 dir0(0,0,1);
  dir0 = TMath::Sqrt(2*Mass_C13*bEn)*dir0;
  TVector3 dir_recon = dir0 - dirR;
  double ene_recon = bEn - epr - dir_recon*dir_recon/Mass_C13/2.;
  JunParticle MM("mm",ene_recon,dir_recon);
  return MM;
}

void JunRebuild::extractAll()
{
}

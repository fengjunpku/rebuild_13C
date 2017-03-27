#include <iostream>
#include <string>
#include "stdio.h"
#include "stdlib.h"

#include <TStopwatch.h>
#include <TString.h>

#include "reDefine.hh"
#include "JunErrors.hh"
#include "JunParMan.hh"
#include "JunDSSDAngle.hh"
#include "JunDataWriter.hh"
#include "JunPIDMan.hh"
#include "JunTreeReader.hh"
#include "JunParticle.hh"
#include "JunLossCorrection.hh"

using namespace std;

int main(int argc,char** argv)
{
  //---------------------------------------------------------------------------
  TStopwatch watch;
  //---------------------------------------------------------------------------
  if(argc != 2)
    MiaoError("Please give a run num !");
  int runnum = atoi(argv[1]);
  //---------------------------Load Setup------------------------------------------------
  JunParMan *parMan = JunParMan::Instance();
  parMan->ReadParFile("setup_13C.par");
  //---------------------------Init Angle------------------------------------------------
  JunDSSDAngle *jAngle = JunDSSDAngle::Instance();
  //---------------------------Load EnergyLoss Correction---------------------------
  JunLossCorrection *lossCorr = new JunLossCorrection();
  lossCorr->addDataFile("He4_in_Al.txt","He4InAl");
  lossCorr->addDataFile("Be9_in_Al.txt","Be9InAl");
  //---------------------------Load PID Cut------------------------------------------------
  JunPIDMan *myPID = new JunPIDMan();
  //---------------------------Read------------------------------------------------
  JunTreeReader *reader = new JunTreeReader(runnum);
  long numOfEntries = reader->GetEntries();
  long onePC = long(numOfEntries/100);
  //---------------------------Write----------------------------------------------------
  JunDataWriter *writer = JunDataWriter::Instance();
  writer->OpenFile(runnum);
  //----------------------------------------------------------------
  printf(" # rebuild sete%04d #\n",runnum);
  for(long ie=0;ie<numOfEntries;ie++)
  {
    if(ie%onePC == 0)
    {
      printf("\r --> Process sete%04d, Events: %ld/%ld %6.1f %%",runnum,ie,numOfEntries,float(ie/onePC));
      fflush(stdout);
    }
    writer->Clear();
    int numOfEventParticle = 0;
    int numOfHe4           = 0;
    int numOfBe9           = 0;
    bool found_alpha = false;
    bool found_break = false;
    bool found_recoi = false;
    reader->GetEntry(ie);
    //l0=======================================
    for(int il=0;il<reader->l0hit;il++)
    {
      if(reader->l0se>0 && myPID->isHe4("l0b",reader->l0b7e[il],reader->l0se) 
        && reader->l0i[il]>=0 && reader->l0j[il]>=0)//found alpha
      {
        double es[3] = {reader->l0se,reader->l0b7e[il],reader->l0w1e[il]};
        double et = lossCorr->GetE(DL_l0,es,3,"He4InAl");
        //double et = reader->l0w1e[il] + reader->l0b7e[il] + reader->l0se;
        double th = jAngle->GetTheta("l0bb7",reader->l0i[il],reader->l0j[il]);
        double ph =   jAngle->GetPhi("l0bb7",reader->l0i[il],reader->l0j[il]);
        JunParticle theAlpha("alpha",et,th,ph);
        found_alpha = true;
        writer->he4 = theAlpha;
        numOfEventParticle++;
        numOfHe4++;
      }
      if(reader->l0se<=0 && myPID->isHe4("l0f",reader->l0w1e[il],reader->l0b7e[il]) 
        && reader->l0i[il]>=0 && reader->l0j[il]>=0)//found alpha
      {
        double es[2] = {reader->l0b7e[il],reader->l0w1e[il]};
        double et = lossCorr->GetE(DL_l0,es,2,"He4InAl");
        //double et = reader->l0w1e[il] + reader->l0b7e[il];
        double th = jAngle->GetTheta("l0bb7",reader->l0i[il],reader->l0j[il]);
        double ph =   jAngle->GetPhi("l0bb7",reader->l0i[il],reader->l0j[il]);
        JunParticle theAlpha("alpha",et,th,ph);
        found_alpha = true;
        writer->he4 = theAlpha;
        numOfEventParticle++;
        numOfHe4++;
      }
      int be_flag = myPID->tellBe("l0",reader->l0w1e[il],reader->l0b7e[il],reader->l0wi[il],reader->l0wj[il]);
      if(be_flag == 9 && reader->l0i[il]>=0 && reader->l0j[il]>=0)//found 9Be
      {
        double es[2] = {reader->l0b7e[il],reader->l0w1e[il]};
        double et = lossCorr->GetE(DL_l0,es,2,"Be9InAl");
        //double et = reader->l0w1e[il] + reader->l0b7e[il];
        double th = jAngle->GetTheta("l0bb7",reader->l0i[il],reader->l0j[il]);
        double ph =   jAngle->GetPhi("l0bb7",reader->l0i[il],reader->l0j[il]);
        JunParticle theBe9("break",et,th,ph);
        writer->be9 = theBe9;
        numOfEventParticle++;
        numOfBe9++;
        if(myPID->isRecoil("front",et,th))//recoil one
        {
          found_recoi = true;
          writer->be9r = theBe9;
        }
        else
        {
          found_break = true;
          writer->be9b = theBe9;
        }
      }
    }
    //r0==============================================================
    for(int il=0;il<reader->r0hit;il++)
    {
      if(reader->r0se>0 && myPID->isHe4("r0b",reader->r0b7e[il],reader->r0se) 
        && reader->r0i[il]>=0 && reader->r0j[il]>=0)//found alpha
      {
        double es[3] = {reader->r0se,reader->r0b7e[il],reader->r0w1e[il]};
        double et = lossCorr->GetE(DL_r0,es,3,"He4InAl");
        //double et = reader->r0w1e[il] + reader->r0b7e[il] + reader->r0se;
        double th = jAngle->GetTheta("r0bb7",reader->r0i[il],reader->r0j[il]);
        double ph =   jAngle->GetPhi("r0bb7",reader->r0i[il],reader->r0j[il]);
        JunParticle theAlpha("alpha",et,th,ph);
        found_alpha = true;
        writer->he4 = theAlpha;
        numOfEventParticle++;
        numOfHe4++;
      }
      if(reader->r0se<=0 && myPID->isHe4("r0f",reader->r0w1e[il],reader->r0b7e[il]) 
        && reader->r0i[il]>=0 && reader->r0j[il]>=0)//found alpha
      {
        double es[2] = {reader->r0b7e[il],reader->r0w1e[il]};
        double et = lossCorr->GetE(DL_r0,es,2,"He4InAl");
        //double et = reader->r0w1e[il] + reader->r0b7e[il];
        double th = jAngle->GetTheta("r0bb7",reader->r0i[il],reader->r0j[il]);
        double ph =   jAngle->GetPhi("r0bb7",reader->r0i[il],reader->r0j[il]);
        JunParticle theAlpha("alpha",et,th,ph);
        found_alpha = true;
        writer->he4 = theAlpha;
        numOfEventParticle++;
        numOfHe4++;
      }
      int be_flag = myPID->tellBe("r0",reader->r0w1e[il],reader->r0b7e[il],reader->r0wi[il],reader->r0wj[il]);
      if(be_flag == 9 && reader->r0i[il]>=0 && reader->r0j[il]>=0)//found 9Be
      {
        double es[2] = {reader->r0b7e[il],reader->r0w1e[il]};
        double et = lossCorr->GetE(DL_r0,es,2,"Be9InAl");
        //double et = reader->r0w1e[il] + reader->r0b7e[il];
        double th = jAngle->GetTheta("r0bb7",reader->r0i[il],reader->r0j[il]);
        double ph =   jAngle->GetPhi("r0bb7",reader->r0i[il],reader->r0j[il]);
        JunParticle theBe9("break",et,th,ph);
        found_break = true;
        writer->be9 = theBe9;
        numOfEventParticle++;
        numOfBe9++;
        if(myPID->isRecoil("front",et,th*TMath::RadToDeg()))//recoil one
        {
          found_recoi = true;
          writer->be9r = theBe9;
        }
        else
        {
          found_break = true;
          writer->be9b = theBe9;
        }
      }
    }
    //=================im===
    if(found_break && numOfHe4==1)
    {
      double ep1 = writer->he4.energy;
      double ep2 = writer->be9b.energy;
      TVector3 dir1 = TMath::Sqrt(2*Mass_He4*ep1)*(writer->he4.direction);
      TVector3 dir2 = TMath::Sqrt(2*Mass_Be9*ep2)*(writer->be9b.direction);
      TVector3 dir_recon = dir1 + dir2;
      double ene_recon = ep1 + ep2 - dir_recon*dir_recon/Mass_C13/2.;
      JunParticle IM("im",ene_recon,dir_recon);
      writer->im = IM;
    }
    //================mm====
    if(found_recoi)
    {
      double bEn = 65;//*MeV
      double epr = writer->be9r.energy;
      TVector3 dirR = TMath::Sqrt(2*Mass_Be9*epr)*(writer->be9r.direction);
      TVector3 dir0(0,0,1);
      dir0 = TMath::Sqrt(2*Mass_C13*bEn)*dir0;
      TVector3 dir_recon = dir0 - dirR;
      double ene_recon = bEn - epr - dir_recon*dir_recon/Mass_C13/2.;
      JunParticle MM("mm",ene_recon,dir_recon);
      writer->mm = MM;
    }
    //====
    if(numOfEventParticle>0)
    {
      writer->num    = numOfEventParticle;
      writer->numHe4 = numOfHe4;
      writer->numBe9 = numOfBe9;
      writer->Fill();
    }
  }
  JunDataWriter::Instance()->Record();


  //---------------------------------------------------------------------------
  printf("\n ======End of  %04d ====\n",runnum);
  printf("   ==> %04d CPU_Time: %s%f%s s, RealTime: %s%f%s s\n",runnum,RED,watch.CpuTime(),COLOR_END,RED,watch.RealTime(),COLOR_END);
}

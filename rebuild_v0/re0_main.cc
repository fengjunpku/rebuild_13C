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
  //---------------------------Load PID Cut------------------------------------------------
  JunPIDMan *myPID = new JunPIDMan();
  //---------------------------Read------------------------------------------------
  JunTreeReader *reader = new JunTreeReader(runnum);
  long numOfEntries = reader->GetEntries();
  long onePC = long(numOfEntries/100);
  //---------------------------Write----------------------------------------------------
  JunDataWriter::Instance()->OpenFile(runnum);
  //----------------------------------------------------------------
  printf(" # rebuild sete%04d #\n",runnum);
  for(long ie=0;ie<numOfEntries;ie++)
  {
    if(ie%onePC == 0)
    {
      printf("\r --> Process sete%04d, Events: %ld/%ld %6.1f %%",runnum,ie,numOfEntries,float(ie/onePC));
      fflush(stdout);
    }
    int numOfEventParticle = 0;
    bool found_alpha = false;
    bool found_break = false;
    //bool found_recoi = false;
    reader->GetEntry(ie);
    //l0
    for(int il=0;il<reader->l0hit;il++)
    {
      if(reader->l0se>0 && myPID->isHe4("l0b",reader->l0b7e[il],reader->l0se) 
        && reader->l0i[il]>=0 && reader->l0j[il]>=0)//found alpha
      {
        double et = reader->l0w1e[il] + reader->l0b7e[il] + reader->l0se;
        double th = jAngle->GetTheta("l0bb7",reader->l0i[il],reader->l0j[il]);
        double ph = jAngle->GetPhi("l0bb7",reader->l0i[il],reader->l0j[il]);
        JunParticle theAlpha("alpha",et,th,ph);
        found_alpha = true;
        JunDataWriter::Instance()->alpha = theAlpha;
        numOfEventParticle++;
        //break;//found one partilce on one tele!!!
      }
      if(reader->l0se<=0 && myPID->isHe4("l0f",reader->l0w1e[il],reader->l0b7e[il]) 
        && reader->l0i[il]>=0 && reader->l0j[il]>=0)//found alpha
      {
        double et = reader->l0w1e[il] + reader->l0b7e[il];
        double th = jAngle->GetTheta("l0bb7",reader->l0i[il],reader->l0j[il]);
        double ph =   jAngle->GetPhi("l0bb7",reader->l0i[il],reader->l0j[il]);
        JunParticle theAlpha("alpha",et,th,ph);
        found_alpha = true;
        JunDataWriter::Instance()->alpha = theAlpha;
        numOfEventParticle++;
        //break;//found one partilce on one tele!!!
      }
      int be_flag = myPID->tellBe("l0",reader->l0w1e[il],reader->l0b7e[il],reader->l0wi[il],reader->l0wj[il]);
      if(be_flag == 9 && reader->l0i[il]>=0 && reader->l0j[il]>=0)//found 9Be
      {
        double et = reader->l0w1e[il] + reader->l0b7e[il];
        double th = jAngle->GetTheta("l0bb7",reader->l0i[il],reader->l0j[il]);
        double ph =   jAngle->GetPhi("l0bb7",reader->l0i[il],reader->l0j[il]);
        JunParticle theBe9("break",et,th,ph);
        found_break = true;
        JunDataWriter::Instance()->breakup = theBe9;
        numOfEventParticle++;
        //break;//found one partilce on one tele!!!
      }
    }
    //====
    if(numOfEventParticle>0)
    {
      JunDataWriter::Instance()->pNum = numOfEventParticle;
      JunDataWriter::Instance()->Fill();
    }
  }
  JunDataWriter::Instance()->Record();


  //---------------------------------------------------------------------------
  printf("\n ======End of  %04d ====\n",runnum);
  printf("   ==> CPU_Time: %s%f%s s, RealTime: %s%f%s s\n",RED,watch.CpuTime(),COLOR_END,RED,watch.RealTime(),COLOR_END);
}

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
#include "JunRebuild.hh"
using namespace std;

int main(int argc,char** argv)
{
  //---------------------------------------------------------------------------
  TStopwatch watch;
  //---------------------------------------------------------------------------
  if(argc != 2)
    MiaoError("Please give a run num !");
  int runnum = atoi(argv[1]);
  //=======================================================
  JunRebuild *conFront = new JunRebuild(runnum);
  conFront->Loop();
  conFront->Save();
  //---------------------------------------------------------------------------
  printf("\n ======End of  %04d ====\n",runnum);
  printf("   ==> %04d CPU_Time: %s%f%s s, RealTime: %s%f%s s\n",runnum,RED,watch.CpuTime(),COLOR_END,RED,watch.RealTime(),COLOR_END);
}
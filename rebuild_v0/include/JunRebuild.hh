#ifndef JUNREBUILD_HH
#define JUNREBUILD_HH 1

#include "reDefine.hh"
#include "JunErrors.hh"
#include "JunParMan.hh"
#include "JunDSSDAngle.hh"
#include "JunDataWriter.hh"
#include "JunPIDMan.hh"
#include "JunTreeReader.hh"
#include "JunParticle.hh"
#include "JunLossCorrection.hh"

class JunRebuild
{
public:
  JunRebuild(int runnum);
  virtual ~JunRebuild();
  void Loop();
  void Save();
private:
  void Reset();
  void anaT0(const string tname);
  int nT0He4(const string tname,double *e,int *ij,bool &matchSSD);
  int nT0Be9(const string tname,double *e,int *ij,int *wij);
  void reQIM();
  void reIM();
  void reMM();
  void Fill();
  JunLossCorrection *ploss;
  JunTreeReader     *pread;
  JunDataWriter     *pwrite;
  JunPIDMan         *pid;
  JunDSSDAngle      *pAngle;
  int runno;
  int numTotal;
  int numOfHe4;
  int numOfBe9;
  int nRecoiBe9;
  int nBreakBe9;
};
#endif
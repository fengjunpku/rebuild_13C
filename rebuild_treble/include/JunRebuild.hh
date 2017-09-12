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

using namespace reDef;

class JunRebuild
{
public:
  JunRebuild(int runnum);
  virtual ~JunRebuild();
  void Loop();
  void Save();
private:
  void Reset();
  void Fill();
  //T0
  void anaT0(const string tname);
  int nT0He4(const string tname,double *e,int *wij,int *ij,double time,bool &matchSSD);
  int nT0Be9(const string tname,double *e,int *wij,int *ij,double time);
  int nT0More(const string tname,double *e,int *wij,double time);
  //T1
  void anaT1(const string tname);
  int nT1He4(const string tname,double *e,int *wij,double time,bool &matchSSD);
  int nT1More(const string tname,double *e,int *wij,double time);
  //trigger
  void getTriggerInfo();
  //rebuild
  void invariantMass_treble();
  void missingMass_treble();
  void invariantMass_double();
  void invariantMass_bebe();
  void missingMass_bebe();
  void invariantMass_doutre();
  void missingMass_doutre();
  JunParticle getIM(JunParticle break_he,JunParticle break_be);
  JunParticle getMM(JunParticle recoil);
  //
  JunLossCorrection *ploss;
  JunTreeReader     *pread;
  JunDataWriter     *pwrite;
  JunPIDMan         *pid;
  JunDSSDAngle      *pAngle;
  JunParticle lastBe;
  int runno;
  int numTotal;
  int numOfHe4;
  int numOfBe9;
  int numOfT0H;
  int numOfT1H;
  int nRecoiBe9;
  int nBreakBe9;
};
#endif

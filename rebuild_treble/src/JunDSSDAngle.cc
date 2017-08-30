#include "JunDSSDAngle.hh"

JunDSSDAngle* JunDSSDAngle::thisAngle=NULL;

JunDSSDAngle::JunDSSDAngle()
{
  JunParMan *jParMan = JunParMan::Instance();
  vector<string> dssdNames = jParMan->GetAllDSSDName();
  for(auto name = dssdNames.begin();name != dssdNames.end();++name)
  {
    Par_DSSD *dssdPar = jParMan->GetDSSDPar(*name);
    double perX = dssdPar->width/dssdPar->vstrips;
    double perY = dssdPar->heigh/dssdPar->hstrips;
    int ni = dssdPar->vstrips;
    int nj = dssdPar->hstrips;
    double offx = dssdPar->xoff;
    double offy = dssdPar->yoff;
    for(int i=0;i<ni;++i)
    {
      for(int j=0;j<nj;++j)
      {
        char buff[48];
        sprintf(buff,"%s_%02d_%02d",(*name).c_str(),i,j);
        string varName=string(buff);
        TVector3 position(0,0,0);
        position[0]=((ni-1)/2.-i)*perX+offx;
        position[1]=((nj-1)/2.-j)*perY+offy;
        position[2]=dssdPar->distance;
        position.RotateY(dssdPar->angle*TMath::DegToRad());
        mapOfTheta[varName]=position.Theta();
        mapOfPhi[varName]=position.Phi();
      }
    }
  }
  InitDMapForFrontDSSD();
}

double JunDSSDAngle::GetTheta(string dssdname,int i,int j)
{
  char buff[48];
  sprintf(buff,"%s_%02d_%02d",dssdname.c_str(),i,j);
  string varName=string(buff);
  if(!mapOfTheta.count(varName))
    MiaoError("Miao! Error: Key: "+varName+" is not in phi Map!");
  return mapOfTheta[varName];
}

double JunDSSDAngle::GetPhi(string dssdname,int i,int j)
{
  char buff[48];
  sprintf(buff,"%s_%02d_%02d",dssdname.c_str(),i,j);
  string varName=string(buff);
  if(!mapOfPhi.count(varName))
    MiaoError("Miao! Error: Key: "+varName+" is not in phi Map!");
  return mapOfPhi[varName];
}

double JunDSSDAngle::RealTheta(string dssdname,double x,double y)
{
  JunParMan *jParMan = JunParMan::Instance();
  Par_DSSD *dssdPar = jParMan->GetDSSDPar(dssdname);
  if(dssdPar==NULL) MiaoError("Miao! Error: Key: "+dssdname+" is not in phi Map!");
  TVector3 position(0,0,0);
  position[0]=-x;
  position[1]=-y;
  position[2]=dssdPar->distance;
  position.RotateY(dssdPar->angle/180.*TMath::Pi());
  return position.Theta();
}

double JunDSSDAngle::RealPhi(string dssdname,double x,double y)
{
  JunParMan *jParMan = JunParMan::Instance();
  Par_DSSD *dssdPar = jParMan->GetDSSDPar(dssdname);
  if(dssdPar==NULL) MiaoError("Miao! Error: Key: "+dssdname+" is not in phi Map!");
  TVector3 position;
  position[0]=-x;
  position[1]=-y;
  position[2]=dssdPar->distance;
  position.RotateY(dssdPar->angle/180.*TMath::Pi());
  return position.Phi();
}

int JunDSSDAngle::Check(string dssdname,int i,int j,double x,double y)
{
  JunParMan *jParMan = JunParMan::Instance();
  Par_DSSD *dssdPar = jParMan->GetDSSDPar(dssdname);
  if(dssdPar==NULL) MiaoError("Miao! Error: Key: "+dssdname+" is not in phi Map!");
  TVector3 position(0,0,0);
  position[0]=-x;
  position[1]=-y;
  position[2]=dssdPar->distance;
  position.RotateY(dssdPar->angle/180.*TMath::Pi());
  //---------
  char buff[48];
  sprintf(buff,"%s_%02d_%02d",dssdname.c_str(),i,j);
  string varName=string(buff);
  if(!mapOfPhi.count(varName))
    MiaoError("Miao! Error: Key: "+varName+" is not in phi Map!");
  //----------------
  double perX = dssdPar->width/dssdPar->vstrips;
  double perY = dssdPar->heigh/dssdPar->hstrips;
  int ni = dssdPar->vstrips;
  int nj = dssdPar->hstrips;
  int ci = x/perX+0.5*ni;
  int cj = y/perY+0.5*nj;
  cout<<" "<<(mapOfTheta[varName]-position.Theta())/TMath::Pi()*180<<" ";
  //cout<<" "<<i<<" "<<j<<" "<<x<<" "<<y<<" ";
  if(i==ci&&j==cj) 
    return 1;
  else 
    return 0;
}

JunDSSDAngle::~JunDSSDAngle()
{
  cout<<"End of Angle Manager!"<<endl;
}

JunDSSDAngle* JunDSSDAngle::Instance()
{
  if(thisAngle==NULL)
    thisAngle = new JunDSSDAngle();
  return thisAngle;
}

void JunDSSDAngle::InitDMapForFrontDSSD()
{
  JunParMan *jParMan = JunParMan::Instance();
  //only for r0 and l0
  TString tname[2] = {"l0","r0"};
  TString dname[2] = {"w1","bb7"};
  for(int nt=0;nt<2;nt++)
  {
    TString wname = tname[nt] + dname[0];
    TString bname = tname[nt] + dname[1];
    Par_DSSD *w1Par = jParMan->GetDSSDPar(string(wname));
    Par_DSSD *b7Par = jParMan->GetDSSDPar(string(bname));
    double wdistance = w1Par->distance;
    double bdistance = b7Par->distance;
    int wi = w1Par->vstrips;
    int wj = w1Par->hstrips;
    int bi = b7Par->vstrips;
    int bj = b7Par->hstrips;
    double wdX = w1Par->width/w1Par->vstrips;
    double wdY = w1Par->heigh/w1Par->hstrips;
    double bdX = b7Par->width/b7Par->vstrips;
    double bdY = b7Par->heigh/b7Par->hstrips;
    double wxof = w1Par->xoff;
    double wyof = w1Par->yoff;
    double bxof = b7Par->xoff;
    double byof = b7Par->yoff;
    for(int i1=0;i1<wi;i1++)
    {
      for(int j1=0;j1<wj;j1++)
      {
        for(int i2=0;i2<bi;i2++)
        {
          for(int j2=0;j2<bj;j2++)
          {
            char buff[48];
            sprintf(buff,"%s_%02d_%02d_%02d_%02d",tname[nt].Data(),i1,j1,i2,j2);
            string varName=string(buff);
            if(i2<2*i1-3||i2>2*i2+3||j2<2*j1-3||j2>2*j1+3)
            {
              dMapTheta[varName] = -1000;
              dMapPhi[varName] = -1000;
              continue;
            }
            double wL = (i1-8)*wdX*bdistance/wdistance + wxof;
            double wR = (i1-7)*wdX*bdistance/wdistance + wxof;
            double wD = (j1-8)*wdY*bdistance/wdistance + wyof;
            double wU = (j1-7)*wdY*bdistance/wdistance + wyof;
            double bL = (i2-16)*bdX + bxof;
            double bR = (i2-15)*bdX + bxof;
            double bD = (j2-16)*bdY + byof;
            double bU = (j2-15)*bdY + byof;
            if(bR<wL||bL>wR||bU<wD||bD>wU)
            {
              dMapTheta[varName] = -1000;
              dMapPhi[varName] = -1000;
              continue;
            }
            double ll = (wL>bL)?wL:bL;
            double rr = (wR<bR)?wR:bR;
            double dd = (wD>bD)?wD:bD;
            double uu = (wU<bU)?wU:bU;
            TVector3 position(0,0,0);
            position[0] = (-ll-rr)/2.;
            position[1] = (-uu-dd)/2.;
            position[2] = bdistance;
            position.RotateY(b7Par->angle*TMath::DegToRad());
            dMapTheta[varName] = position.Theta();
            dMapPhi[varName] = position.Phi();
            //cout<<varName<<" : "<<position.Theta()*TMath::RadToDeg()<<" "<<position.Phi()*TMath::RadToDeg()<<endl;
          }
        }
      }
    }
  }
}

double JunDSSDAngle::GetTheta(string telename,int wi,int wj,int i,int j)
{
  char buff[48];
  sprintf(buff,"%s_%02d_%02d_%02d_%02d",telename.c_str(),wi,wj,i,j);
  string varName=string(buff);
  if(!dMapTheta.count(varName))
    MiaoError("Miao! Error: Key: "+varName+" is not in dTheta Map!");
  return dMapTheta[varName];
}

double JunDSSDAngle::GetPhi(string telename,int wi,int wj,int i,int j)
{
  char buff[48];
  sprintf(buff,"%s_%02d_%02d_%02d_%02d",telename.c_str(),wi,wj,i,j);
  string varName=string(buff);
  if(!dMapPhi.count(varName))
    MiaoError("Miao! Error: Key: "+varName+" is not in dPhi Map!");
  return dMapPhi[varName];
}
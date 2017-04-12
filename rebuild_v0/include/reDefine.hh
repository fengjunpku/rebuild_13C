#ifndef REDEFINE_HH
#define REDEFINE_HH 1

#include <TString.h>
#include <string>
#include <map>

static const TString in_dir = "/data/d2/CIAE_Jan2017_13C/setefile/";
static const TString ou_dir = "/data/d2/CIAE_Jan2017_13C/reV0/";

static const double Mass_C13 = 12109.5;//*MeV
static const double Mass_Be9 = 8392.75;//*MeV
static const double Mass_He4 = 3727.38;//*MeV

//thickness of dead layer [um]
static const double DL_l0[3] = {1,1,1};//w1 bb7 ssd
static const double DL_r0[3] = {1,1,1};//w1 bb7 ssd
static const double DL_l1[2] = {1,1};//w1 ssd
static const double DL_r1[2] = {1,1};//w1 ssd

#endif
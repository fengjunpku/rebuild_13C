void keV2MeV()
{
	//TString filename("l0he4_w.root");
  TString filename("r0he4_w.root");
	TFile *file = TFile::Open(filename.Data());
	TCutG *cg = NULL;
	TCutG *cutg = new TCutG();
	file->GetObject("CUTG",cg);
	int nPoint = cg->GetN();
	for(int i=0;i<nPoint;i++)
	{
		double x,y;
		cg->GetPoint(i,x,y);
		cutg->SetPoint(i,x*1e-3,y*1e-3);
	}
	file->Close();
	cutg->SetName("CUTG");
	cutg->SaveAs("MeV_cut.root");
}

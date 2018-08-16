void ana7(){
  for(int filenum = 63;filenum<96;filenum++){
  gStyle->SetOptStat(200);
  gStyle->SetStatH(0.30);
  gStyle->SetStatW(0.60);
  //gStyle->SetOptStat(0);
    TH1D *h = new TH1D("h30", "Ch30;p.e.;Events",150, 0, 30);
    TH1D *horig = new TH1D("horig30" ,"Ch30;p.e.;Events",30, 0, 30);

  auto f1 = TFile::Open(Form("sc00%d.root",filenum));
  auto tr = dynamic_cast<TTree*>(f1->Get("tree"));

  Int_t adc1[32], adc2[32];
  tr->SetBranchAddress("adc", &adc1);
  tr->SetBranchAddress("adc_l", &adc2);
  ifstream ifs(Form("gainfitpow1rightloop%d.txt",filenum));
  Double_t a,b;
  ifs >> a >> b;
  for(int ev=0; ev < tr->GetEntries(); ev++) {
    tr->GetEntry(ev);

    int yield;
      yield = (int)(adc1[30] - a)/b;
      horig->Fill(yield);
  }
  TF1 *tf = new TF1("poison","[0]*TMath::Poisson(x,[1])",0,100);
  //cout << facctorialMethod(5)<< endl;
  tf->SetParameter(0,1600);
  tf->SetParameter(1,5);
  horig->Draw();
  horig->Fit("poison","","",0,20);
  ofstream ofs(Form("pepoimean%d.txt",filenum));
  ofs << Form("sc00%d",filenum) << " " << tf->GetParameter(1) << " " << tf->GetParError(1) <<endl;
  }
}

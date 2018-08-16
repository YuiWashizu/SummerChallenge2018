void ana5(){
  for(int filenum = 73;filenum<96;filenum++){
  gStyle->SetOptStat(200);
  gStyle->SetStatH(0.30);
  gStyle->SetStatW(0.60);
  //gStyle->SetOptStat(0);
    TH1D *h = new TH1D("h30", "Ch30;p.e.;Events",150, 0, 30);
    TH1D *horig = new TH1D("horig30" ,"Ch30;p.e.;Events",150, 0, 30);

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

    double yield;
      yield = (adc1[30] - a)/b;
      horig->Fill(yield);
  }
  horig->Draw();
  ofstream ofs(Form("pemean%d.txt",filenum));
  ofs << Form("sc00%d",filenum) << " " << horig->GetMean() << " " << horig->GetMeanError() <<endl;
  }
}

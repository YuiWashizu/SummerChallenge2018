void MPPCpeFit(){
  for(int filenum=63;filenum < 96;filenum++){
  const int fitch = 30;
  const int pedadc = 810;
  const int fitpenumber = 3;
  const int fitpeadc = 1080;
  const int fitpemax = 5;
  const int daitaifitpeevent = 45;
  auto f1 = TFile::Open(Form("sc00%d.root",filenum));
  auto tr = dynamic_cast<TTree*>(f1->Get("tree"));
  TH1D* histforfit = new TH1D("hff","hff",1000,0,4000);
  Int_t adc1[32], adc2[32];
  tr->SetBranchAddress("adc", &adc1);
  tr->SetBranchAddress("adc_l", &adc2);
  for(int ev=0; ev < tr->GetEntries(); ev++) {
    tr->GetEntry(ev);
    histforfit->Fill(adc1[fitch]);
  }
  const int fitwidth = 15;
  const int daitainosigma = 7;
  const int fitloopnumber = fitpemax-fitpenumber+1;
  TF1* fitgaus[fitloopnumber];
  int fitzakkuripetoadc = (fitpeadc-pedadc)/fitpenumber;
  ofstream ofs(Form("gain%d.txt",filenum));
  //ofstream ofs(Form("gainl%d.txt",fitch));
  ofs << 0 << " " << "\t" << 15000 << " " << "\t" << pedadc << " " << "\t" << 7 << endl;
  for(int i=0;i<fitloopnumber;i++){
    fitgaus[i] = new TF1("fitgaus","[0]*TMath::Gaus(x,[1],[2])",0,4000);
    fitgaus[i]->SetParameter(0,daitaifitpeevent);
    int j =i*fitzakkuripetoadc;
    fitgaus[i]->SetParameter(1,fitpeadc+j);
    fitgaus[i]->SetParameter(2,daitainosigma);
    histforfit->Fit("fitgaus","","",fitpeadc+j-fitwidth,fitpeadc+j+fitwidth);
    Double_t meanevent = fitgaus[i]->GetParameter(0);
    Double_t mean = fitgaus[i]->GetParameter(1);
    Double_t sigma = fitgaus[i]->GetParameter(2);
    if(sigma<0.01){
      sigma=0.0001;
    }
    ofs << fitpenumber+i <<  " " << "\t" << meanevent << " " << "\t" << abs(mean) << " " << "\t" << abs(sigma) << endl;
  }
  ifstream ifs(Form("gain%d.txt",filenum));
  //ifstream ifs(Form("gainl%d.txt",fitch));
  ofstream ofs1(Form("secondgain%d.txt",filenum));
  //ofstream ofs1(Form("secondgainl%d.txt",fitch));
  int i=0;
  int a;
  double b,c,d;
  while(ifs>>a>>b>>c>>d){
    fitgaus[i] = new TF1("fitgaus","[0]*TMath::Gaus(x,[1],[2])",0,4000);
    fitgaus[i]->SetParameter(0,b);
    fitgaus[i]->SetParameter(1,c);
    fitgaus[i]->SetParameter(2,d);
    histforfit->Fit("fitgaus","","",c-fitwidth,c+fitwidth);
    Double_t meanevent = fitgaus[i]->GetParameter(0);
    Double_t mean = fitgaus[i]->GetParameter(1);
    Double_t sigma = fitgaus[i]->GetParameter(2);
    if(sigma<0.01){
      sigma=0.0001;
    }
    ofs1 << a << " " << "\t" << mean << " " << "\t" << sigma << " " << "\t" << meanevent << endl;
  i=i+1;
  }
}
}

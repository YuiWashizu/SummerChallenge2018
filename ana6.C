void ana6(){
  for(int filenum=63;filenum<96;filenum++){
  ofstream ofs(Form("gainfitpow1rightloop%d.txt",filenum));
    vector<Double_t> x,y,ye;
    Double_t xx,yy,yeye,hoge0,hoge1;

    ifstream ifs(Form("secondgain%d.txt",filenum));
      while(ifs>>xx>>yy>>yeye>>hoge0){
      x.push_back(xx);
      y.push_back(yy);
      ye.push_back(yeye);
      }
      ifs.close();
    Double_t* xpointer=&(x.at(0));
    Double_t* ypointer=&(y.at(0));
    Double_t* yepointer=&(ye.at(0));
    TGraphErrors* tg = new TGraphErrors(x.size(),xpointer,ypointer,0,yepointer);
    TF1* f = new TF1("f","[0]+[1]*x",-1,x.size()+1);
    f->SetParameter(0,0);
    f->SetParameter(1,0);
    tg->Fit("f","Q");
    Double_t p0 = f->GetParameter(0);
    Double_t p1 = f->GetParameter(1);
    //Double_t p2 = f[i]->GetParameter(2);
    Double_t chi2 = f->GetChisquare();
    ofs << p0 << " " << "\t" << p1 << " " << "\t" << chi2/x.size() << " " << "\t" << x.size() << endl;
  tg->Draw("AP");
  gStyle->SetOptFit(1111);
  /*tg->SetTitle("plateADCtoPE;p.e.;adc");
  tg->GetXaxis()->SetLimits(-2,11);
  tg->SetMaximum(1500);
  tg->SetMinimum(800);*/
  }
}

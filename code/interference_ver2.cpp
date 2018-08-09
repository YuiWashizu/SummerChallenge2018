#include <TFile.h>
#include <TH1.h>
#include <TCanvas.h>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <string>
#include <sstream>
#include <TF1.h>
#include <TTree.h>
#include <TStyle.h>
#include <TText.h>
#include <unistd.h>

using namespace std;

  const int ch = 30;
  const int pedestal_adc = 805;
  const int first_adc = 860;
  const int fit_sigma = 7;
  const int fit_peak = 45;
  const int fit_width = 20;
  int first_position;
  int stage_step;
  int peak_number;
  int begin_file_number;
  int last_file_number;
  string output_filename;
  int mode;


 void interference_ver2(){
  vector<double> x_pos;
  vector<double> pe_mean;
  vector<double> pe_mean_err;
  //determine measurement condition
  cout<<"please input first file number"<<endl;
  cin>>begin_file_number;
  cout<<"please input last file number"<<endl;
  cin>>last_file_number;
  cout<<"please input the number of fit peak"<<endl;
  cout<<"(when low photon,recommend 1)"<<endl;
  cin>>peak_number;
  cout<<"please input first stage position"<<endl;
  cin>>first_position;
  cout<<"please input each stage step"<<endl;
  cin>>stage_step;
  cout<<"please input output root file name"<<endl;
  cin>>output_filename;
  cout<<"please select analysis mode"<<endl;
  cout<<"mode1 = poisson fitting,mode2 = Entries analysis"<<endl;
  cout<<"(when low photon:recommend 2,other case:recommend 1)"<<endl;
  cout<<"please type 1 or 2"<<endl;
  cin>>mode;

  TFile * output_file = new TFile(output_filename.c_str(),"RECREATE");

  //each position fitting
  for(int filenumber = begin_file_number;filenumber <= last_file_number;filenumber++){ 
   auto tf1 = TFile::Open(Form("./data/sc0%d.root",filenumber));
   auto tr1 = dynamic_cast<TTree*>(tf1->Get("tree"));
   TH1D * adc_hist = new TH1D(Form("adc_hist%d",filenumber),Form("adc_hist%d",filenumber),4000,0,4000);
   int adc[32];
   tr1->SetBranchAddress("adc",&adc);
   for(int event = 0;event < tr1->GetEntries();event++){
    tr1->GetEntry(event);
    adc_hist->Fill(adc[ch]);
   }
   output_file->cd();
   adc_hist->Draw();
   adc_hist->Write();

   vector<double> fit_adc_mean;
   vector<double> fit_adc_sigma;
   vector<double> gaus_x;
   for(int j = 0;j <= peak_number;j++){
   TF1 *fitgaus = new TF1("fitgaus","[0]*TMath::Gaus(x,[1],[2])",0,4000);
   fitgaus->SetParameter(0,fit_peak);
   fitgaus->SetParameter(1,pedestal_adc + j*(first_adc - pedestal_adc));
   fitgaus->SetParameter(2,fit_sigma);
   adc_hist->Fit("fitgaus","","",pedestal_adc + j*(first_adc - pedestal_adc) - fit_width,pedestal_adc + j*(first_adc - pedestal_adc) + fit_width);
   fit_adc_mean.push_back(fitgaus->GetParameter(1));
   fit_adc_sigma.push_back(fitgaus->GetParameter(2));
   gaus_x.push_back(static_cast<double>(j));
  }

   TCanvas * c1 = new TCanvas(Form("line_fit%d",filenumber),Form("line_fit%d",filenumber),1600,900);
   TGraphErrors* g1 = new TGraphErrors(peak_number + 1,&(gaus_x.at(0)),&(fit_adc_mean.at(0)),0,&(fit_adc_sigma.at(0)));
   TF1 * f1 = new TF1("f1","[0] + [1] * x",-1,peak_number + 1);
   f1->SetParameter(0,0);
   f1->SetParameter(1,0);
   g1->Fit("f1","Q");
   double p0 = f1->GetParameter(0);
   double p1 = f1->GetParameter(1);
   output_file->cd();
   g1->Draw("AP");
   c1->Write();
   delete c1;

   int Entries = 0;
   tf1->cd();
   TH1D * pe_hist = new TH1D(Form("pe_hist%d",filenumber),Form("pe_hist%d",filenumber),30,-0.5,29.5);
   TH1D * pe_hist_sig = new TH1D(Form("pe_hist_sig%d",filenumber),Form("pe_hist_sig%d",filenumber),30,-0.5,29.5);
   for(int event = 0;event < tr1->GetEntries();event++){
    tr1->GetEntry(event);
    pe_hist->Fill((adc[ch] - p0)/p1);
    if((adc[ch] - p0)/p1 > 0.5){
   	 Entries++;
   	 pe_hist_sig->Fill((adc[ch] - p0)/p1);
    }
   }
   
   output_file->cd();
   TCanvas * c2 = new TCanvas(Form("pe_hist%d",filenumber),Form("pe_hist%d",filenumber),1600,900);
   x_pos.push_back(0.002*(first_position + stage_step*(filenumber - begin_file_number)));
   TF1 * fit_poisson = new TF1("poisson","[0]*TMath::Poisson(x,[1])",0,100);
   fit_poisson->SetParameter(0,10000);
   fit_poisson->SetParameter(1,3);
   pe_hist->SetFillColor(kBlue);
   pe_hist->Draw();
   
   if(mode == 1){
    pe_hist->Fit("poisson","","",0,20);
    pe_mean.push_back(fit_poisson->GetParameter(1));
    pe_mean_err.push_back(fit_poisson->GetParError(1));
   }else{
    pe_mean.push_back(Entries);
    pe_mean_err.push_back(TMath::Sqrt(Entries));
    pe_hist_sig->SetFillColor(kRed);
    pe_hist_sig->Draw("same");
    c2->RedrawAxis();
   }
   output_file->cd();
   c2->Write();

   delete c2;
   delete adc_hist;
   delete g1;
   delete f1;
   delete pe_hist;
   delete fit_poisson;
  }

  double * xpointer = &(x_pos.at(0));
  double * ypointer = &(pe_mean.at(0));
  double * y_errpointer = &(pe_mean_err.at(0));
  TCanvas * c1 = new TCanvas("interference","interference",1600,900);
  TGraphErrors *g2 = new TGraphErrors(x_pos.size(),xpointer,ypointer,0,y_errpointer);
  g2 -> Draw("AP");
  if(mode == 1){
   g2->SetTitle("interferece;position[mm];Average photon");
   }else{
   g2->SetTitle("interferece;position[mm];Entries > 0.5");
   }
  c1->Write();
  c1->SaveAs("interference.png");
  delete c1;
 }

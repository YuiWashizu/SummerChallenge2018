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
   const int first_position = 34000;
   const int stage_step = 125;

 void interference(const int begin,const int last){
  vector<double> x_pos;
  vector<double> pe_mean;
  vector<double> pe_mean_err;
  for(int filenumber = begin;filenumber <= last;filenumber++){ 	
   auto tf1 = TFile::Open(Form("./data/sc0%d.root",filenumber));
   auto tr1 = dynamic_cast<TTree*>(tf1->Get("tree"));
   TH1D * adc_hist = new TH1D("adc_hist","adc_hist",4000,0,4000);
   int adc[32];
   tr1->SetBranchAddress("adc",&adc);
   for(int event = 0;event < tr1->GetEntries();event++){
    tr1->GetEntry(event);
    adc_hist->Fill(adc[ch]);
   }
   
   adc_hist->Draw();
   double fit_adc_mean[3];
   double fit_adc_sigma[3];
   for(int j = 0;j < 3;j++){
   TF1 *fitgaus = new TF1("fitgaus","[0]*TMath::Gaus(x,[1],[2])",0,4000);
   fitgaus->SetParameter(0,fit_peak);
   fitgaus->SetParameter(1,pedestal_adc + j*(first_adc - pedestal_adc));
   fitgaus->SetParameter(2,fit_sigma);
   adc_hist->Fit("fitgaus","","",pedestal_adc + j*(first_adc - pedestal_adc) - fit_width,pedestal_adc + j*(first_adc - pedestal_adc) + fit_width);
   fit_adc_mean[j] = fitgaus->GetParameter(1);
   fit_adc_sigma[j] = fitgaus->GetParameter(2);
  }
   sleep(2);

   double gaus_x[3] = {0.0,1.0,2.0};
   TGraphErrors* g1 = new TGraphErrors(3,gaus_x,fit_adc_mean,0,fit_adc_sigma);
   TF1 * f1 = new TF1("f1","[0] + [1] * x",-1,3);
   f1->SetParameter(0,0);
   f1->SetParameter(1,0);
   g1->Fit("f1","Q");
   double p0 = f1->GetParameter(0);
   double p1 = f1->GetParameter(1);

   TH1D * pe_hist = new TH1D("pe_ghist","pe_hist",30,-0.5,29.5);
   for(int event = 0;event < tr1->GetEntries();event++){
   tr1->GetEntry(event);
   pe_hist->Fill((adc[ch] - p0)/p1);
   }

   TF1 * fit_poisson = new TF1("poisson","[0]*TMath::Poisson(x,[1])",0,100);
   fit_poisson->SetParameter(0,10000);
   fit_poisson->SetParameter(1,3);
   pe_hist->Draw();
   pe_hist->Fit("poisson","","",0,20);
   x_pos.push_back(0.002*(first_position + stage_step*(filenumber - begin)));
   pe_mean.push_back(fit_poisson->GetParameter(1));
   pe_mean_err.push_back(fit_poisson->GetParError(1));
   delete adc_hist;
   delete g1;
   delete f1;
   delete pe_hist;
   delete fit_poisson;
  }
  double * xpointer = &(x_pos.at(0));
  double * ypointer = &(pe_mean.at(0));
  double * y_errpointer = &(pe_mean_err.at(0));
  TGraphErrors *g2 = new TGraphErrors(x_pos.size(),xpointer,ypointer,0,y_errpointer);
  g2 -> Draw("AP");
 }

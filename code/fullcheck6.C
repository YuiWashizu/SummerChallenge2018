#include<iostream>
#include<fstream>
#include<sstream>

#include"TH1.h"
#include"TH2.h"
#include"TTree.h"
#include"TFile.h"

void fullcheck6(char* source){//
  std::ifstream ifs(source, std::ios::binary);

  if(!ifs.is_open()){
    std::cout << "no file" << std::endl;
    return;
  }

  //TH1F *tdc[32][16];
  //TH1F *tdc_n[32][16];
  TH1F *adc[32];
  TH1F *adc_l[32];

  int dtdc[32][16];
  int dtdc_n[32][16];
  int dadc[32];
  int dadc_l[32];
  
  std::string fn = string(source);
  const std::string find_fn("dat");  
  const std::string rep_fn("root");
  std::string::size_type pos = 0;
  pos = fn.find(find_fn,pos);
  fn.replace(pos,find_fn.length(),rep_fn); 
  const char* cfn = fn.std::string::c_str();

  TFile *fout = new TFile(cfn,"RECREATE");
  TTree *tree = new TTree("tree","tree");
  //tree->Branch("tdc", dtdc, "tdc[32][16]/I");
  //tree->Branch("tdc_n", dtdc_n, "tdc_n[32][16]/I");
  tree->Branch("adc", dadc, "adc[32]/I");
  tree->Branch("adc_l", dadc_l, "adc_l[32]/I");

  for(int ch = 0; ch<32; ++ch){
    std::ostringstream _name;
    std::ostringstream _name_l;
    _name << "h" << ch;
    _name_l << "hl" << ch;

    std::string ahist_name = _name.str();
    adc[ch] = new TH1F(ahist_name.c_str(), ahist_name.c_str(),
    4000,0,4000);
    std::string alhist_name = _name_l.str();
    adc_l[ch] = new TH1F(alhist_name.c_str(), alhist_name.c_str(),
    4000,0,4000);
    for(int i = 0; i<16; ++i){
    std::ostringstream _name_t;
    std::ostringstream _name_t_n;
    //_name_t << "h" << ch << "_" << i;
    //_name_t_n << "h" << ch << "_" << i << "_n";
    std::string thist_name = _name_t.str();
    //std::string thist_name_n = _name_t_n.str();
    //tdc[ch][i] = new TH1F(thist_name.c_str(), thist_name.c_str(),
    //2000,0,2000);

    //tdc_n[ch][i] = new TH1F(thist_name_n.c_str(), thist_name_n.c_str(),
    //2000,0,2000);
  }

}

int index[32];
int index_n[32];

while(!ifs.eof()){
  int data;
  ifs.read((char*)&data, sizeof(int));

  if((unsigned int)data == 0xffffea0c){
    for(int ch = 0; ch<32; ++ch){
      index[ch] = 0;
      index_n[ch] = 0;
      dadc[ch] = -1;
      for(int m = 0; m<16; ++m){
        dtdc[ch][m] = -1;
        dtdc_n[ch][m] = -1;
      }
    }


    ifs.read((char*)&data, sizeof(int));
    int NofWord = data & 0xffff;

    //skip event counter
    ifs.read((char*)&data, sizeof(int));
    for(int i = 0; i<NofWord; ++i){
      ifs.read((char*)&data, sizeof(int));

      if(0xcc == ((data >> 24) & 0xff)){
        int channel = (data >> 16) & 0xff;
        int val = data & 0xfff;
        std::cout << "ch " << channel << " " << val << std::endl;
        if(0 == ((data >> 15) & 0x1)){
          //tdc[channel][index[channel]]->Fill(val);
          dtdc[channel][index[channel]] = val;
          index[channel]++;
        }

        if(1 == ((data >> 15) & 0x1)){
          //tdc_n[channel][index_n[channel]]->Fill(val);
          dtdc_n[channel][index_n[channel]] = val;
          index_n[channel]++;
        }
      }else if(0x81 == ((data >> 24) & 0xff)){
        int channel = (data >> 16) & 0xff;
        int val = data & 0xfff;
        adc[channel]->Fill(val);
        dadc[channel] = val;
      }
      else if(0x60 == ((data >> 24) & 0xff)){
        int channel = (data >> 16) & 0xff;
        int val = data & 0xfff;
        adc_l[channel]->Fill(val);
        dadc_l[channel] = val;
      }
    }

    tree->Fill();
  }
}

fout->Write();
fout->Close();
delete fout;
fout = NULL;

return;
}

void kanshow7(){
  char a[10];
  vector<double> b,c,x;
  double bb,cc;
  //cout << "test1" << endl;
  for(int filenum = 63;filenum <96;filenum++){
  ifstream ifs(Form("pepoimean%d.txt",filenum));
  //cout << "test2" << endl;
  while(!ifs.eof()){
  ifs>>a>>bb>>cc;
  //cout << a << " " << bb << " " << cc << endl;
  b.push_back(bb);
  c.push_back(cc);
  if(filenum<72){
  x.push_back(125*(filenum-63));
  }
  if(filenum>71){
  x.push_back(125*(71-filenum));
  cout << filenum << " " << bb << endl;
  }
  //cout << "test3" << endl;
 }
 Double_t* xpointer=&(x.at(0));
 Double_t* bpointer=&(b.at(0));
 Double_t* cpointer=&(c.at(0));
 //cout << c[0] << endl;
 TGraphErrors *tg = new TGraphErrors(x.size(),xpointer,bpointer,0,cpointer);
 tg -> Draw("AP");
}
}

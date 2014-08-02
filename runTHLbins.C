
#include "TMAth.h"

void runTHLbins(){
  
  //TFile f("MAFOutput_MPXNtuple_background_8h_later_noHV.root","read");
  //TTree test("Test","a Tree with pixel data");
  //Double_t epix;
  //std::vector<double> *energyPerPixel=0;
  //test.SetBranchAddress("energyPerPixel",&epix);
  //TTree *test = (TTree*)f->Get("Test");
  
  //test->SetBranchAddress("energyPerPixel",&energyPerPixel);
  //Int_t nentries = (Int_t)test->GetEntries();  
  
  Int_t const THL = 729;

  Double_t energy = 0.0;
  Double_t a = -12.1678;
  Double_t b = 764.9263;

  cout << "THL : Energy : TOT" << endl;

  for (Int_t i=THL; i > 600; i--) {
    energy = (i - b)/a;
    cout << i << "   " << energy << "   " << (THL-i) <<  endl;
  }    
}  
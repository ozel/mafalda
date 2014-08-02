
#include "TMAth.h"

void runReadEnergies(){
  
  TFile f("MAFOutput_MPXNtuple_background_8h_later_noHV.root","read");
  //TTree test("Test","a Tree with pixel data");
  //Double_t epix;
  std::vector<double> *energyPerPixel=0;
  //test.SetBranchAddress("energyPerPixel",&epix);
  TTree *test = (TTree*)f->Get("Test");
  
  test->SetBranchAddress("energyPerPixel",&energyPerPixel);
  Int_t nentries = (Int_t)test->GetEntries();  
  double old = 0.0;
  double current;
  double currdiff;
  double olddiff =1.0;
  for (Int_t i=0; i<nentries; i++) {
    test->GetEntry(i);
    //cout << "Event " << i << " number of fired pixels: " << energyPerPixel->size() << endl;
    for (UInt_t j=0; j<energyPerPixel->size(); ++j) {
       current = energyPerPixel->at(j);
       currdiff = fabs(current - old);
       if ( (currdiff < olddiff) /* && (currdiff != 0) */){
          cout << currdiff << endl;
          olddiff = currdiff;
       }
       old = current;
    }   
    //hpx->Fill(px);
    //hpxpy->Fill(px,py);
  }  
  f.Close();
  
}  
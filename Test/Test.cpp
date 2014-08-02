
/**
 * Created automatically with MAFalda (Mi 23 Jul 2014 16:04:23 CEST)
 * MAFalda Author:  John Idarraga <idarraga@cern.ch>
 *
 * An example of simple processing without clustering.
 */

#ifndef __Test_cpp
#define __Test_cpp

#include "Test.h"
#include "MAFTools.h"
#include <unistd.h>

using namespace MSG;



ClassImp(Test)

Test::Test() : MediPixAlgo(), CalibrationLoader(this) {

  m_myhits.totalHits = 0;
  m_myhits.totalCharge = 0;
  h_x = new TH1D("Energy Histogram", "", 14, 0, 9.89947);
  h_x2 = new TH1D("Energy Histogram2", "", 100, 0, 700);

  m_sizex = 256;
  m_sizey = 256;

  // Global TOT distributions
  m_SingleHit_GlobalEnergyDist = new TH1D("SingleHitEnergyDist","SingleHitEnergyDist",1000, 0, 10);

  // Entries 2D histos
  m_SingleHit_EntriesHisto = new TH2I("entriesSingle", "entriesSingle", m_sizex, 0, m_sizex, m_sizey, 0, m_sizey);

  

}

void Test::Init() {

  // You will get an ntuple file containing a TTree with the name of this
  //  Algorithm.  The branches registered through getMyTree() get registered
  //  in that tree so you can fill them each time Execute() gets called.
  getMyTree()->Branch("myhits", &m_myhits , "totalHits/I:totalCharge");
  getMyTree()->Branch("energyPerPixel", &m_energyPerPixel);


  Log << MSG::INFO << "Init function !" << endreq;

  getMyTree()->Branch("m_SingleHitCoor", &m_SingleHitCoor);
  getMyTree()->Branch("m_SingleHitTOT", &m_SingleHitTOT);
  getMyTree()->Branch("m_SingleHitEnergy", &m_SingleHitEnergy);

  

  //h_x = new TH1D("Energy Histogram", Form("%d", GetFrameId()), 100, 0, 100);

}

void Test::Execute(){

  // REMINDER: Test::Execute() runs once per frame
  //  you may need ro reinitialize variables.
  m_myhits.totalCharge = 0;
  m_myhits.totalHits = 0;

  //if(m_canvas_p1) { delete m_canvas_p1; }


  int xDim = GetMatrixXdim();
  int yDim = GetMatrixYdim();

  int rowItr = 0, colItr = 0, tot = 0;
  double calib_edep = 0.;


  for(colItr = 0; colItr < xDim ; colItr++) {

      for(rowItr = 0; rowItr < yDim ; rowItr++) {

    	  if(GetMatrixElement(colItr, rowItr) > 0) {

  	      // Get the TOT
  	      tot = GetMatrixElement(colItr, rowItr);

  	      // Get the TOT or counts
  	      m_myhits.totalCharge += tot;
  	      m_myhits.totalHits++;

  	      // Get the calibrated energy if the calibration files are loaded in the 
                //  top layer (ROOT Macro used to drive the run).
  	      calib_edep = CalculateAndGetCalibEnergy(colItr, rowItr, tot);

    	      // Push back in the vector for output.  There is no notion of "cluster"
  	      //  in this example.  We are simply storing the energy from all pixels.
  	      m_energyPerPixel.push_back( calib_edep );

          Log << MSG::DEBUG << "TOT:" << tot << " pixel energy:" << calib_edep << endreq;
          
          h_x->Fill(calib_edep);
          //h_x2->Fill(calib_edep);

        }
      }
  }

  int lastObject = GetNumberOfObjectsWithAuthor("SimpleClustering");
  if(lastObject == 0)
    return;

  m_sC = static_cast<SimpleClusterContainer *> ( GetObjectFromAuthor("SimpleClustering", lastObject-1) );
  vector<pair<int, int> > sh = m_sC->GetSingleHits();
  vector<pair<int, int> >::iterator shItr = sh.begin();

  int framewidth = GetWidth();
  int tempTOT;

  for ( ; shItr != sh.end() ; shItr++) {
    
    //crop 2 pixels on all chip edges
    if( ((*shItr).first  > 1) && ((*shItr).first  < 254) &&
        ((*shItr).second > 1) && ((*shItr).second < 254) ) {
      // Store coordinate and TOT to Ntuple
      // <X> and <TOT>
      tempTOT = GetMatrixElement( *shItr );
      m_SingleHitCoor.push_back( MAFTools::XYtoX( *shItr , framewidth ) );
      m_SingleHitTOT.push_back( tempTOT );

      calib_edep = CalculateAndGetCalibEnergy( *shItr, tempTOT);
      m_SingleHitEnergy.push_back(calib_edep);

      // Stats plots. Entries and not TOT.
      m_SingleHit_EntriesHisto->Fill( (*shItr).first, (*shItr).second ); // increase by one

      // Global TOT distribution
      m_SingleHit_GlobalEnergyDist->Fill( calib_edep );
    }
  }

  //h_x->Draw();
  

  Log << MSG::INFO << "Total charge/counts     : " << m_myhits.totalCharge << endreq ;
  Log << MSG::INFO << "Number of active pixels : " << m_myhits.totalHits << endreq ;


  getMyTree()->Fill();

  // Get output variables ready for next frame after Filling the Tree
  m_energyPerPixel.clear();
  // single hit
  m_SingleHitCoor.clear();
  m_SingleHitTOT.clear();
  m_SingleHitEnergy.clear();
}

void Test::Finalize(){


  gStyle->SetOptStat(kTRUE);
  h_x->TH1::SetStats(kTRUE);
  h_x->GetXaxis()->SetLabelSize(0.05);
  h_x->GetYaxis()->SetLabelSize(0.05);
  h_x->GetXaxis()->SetTitleSize(0.05);
  h_x->GetYaxis()->SetTitleSize(0.05);
  //h_x->Write();

  //h_x->Draw();
  m_SingleHit_GlobalEnergyDist->Draw();
  //m_SingleHit_GlobalEnergyDist->Write();
  //m_SingleHit_EntriesHisto->Draw();


  //vh_x.push_back( h_x2 );
  //m_canvas_p1 = DrawInSeparateWindow(vh_x, MSG::DEBUG);
  //h_x2->Draw();
  //m_canvas_p1->Draw();
  //m_canvas_p1->Update();
  

  Log << MSG::INFO << "Finalize function !" << endreq;

}

#endif

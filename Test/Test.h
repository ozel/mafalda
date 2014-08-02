
/**
 * Created automatically with MAFalda (Mi 23 Jul 2014 16:04:23 CEST)
 * MAFalda Author:  John Idarraga <idarraga@cern.ch>
 *
 * An example of simple processing without clustering.
 */

#ifndef __Test_h
#define __Test_h

#include "MPXAlgo/MediPixAlgo.h"
#include "CalibrationLoader/CalibrationLoader.h"
#include "SimpleClustering/SimpleClustering.h"

class Test : public MediPixAlgo , public CalibrationLoader {

public:

  Test();
  virtual ~Test() { };

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();

private:

  // Variables for output
  // First in the form of a C-style structure
  typedef struct {
    Int_t totalHits;
    Int_t totalCharge;
  } score;
  score m_myhits;
  // In the form of a vector
  vector<double> m_energyPerPixel;

  SimpleClusterContainer * m_sC;

  int m_sizex;
  int m_sizey;

  // Map for single hit entries
  vector<int> m_SingleHitCoor;
  vector<int> m_SingleHitTOT;
  vector<double> m_SingleHitEnergy;

  // Number of entries histo
  TH2I * m_SingleHit_EntriesHisto;

  // Single hit TOT global distributions
  TH1D * m_SingleHit_GlobalEnergyDist;

  TCanvas * m_canvas_p1;

  TH1D * h_x;
  TH1D * h_x2;

  vector<TH1 *> vh_x;  

  ClassDef(Test, 1)
};

#endif

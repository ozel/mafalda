
/**
 * Created automatically with MAFalda (Fri Nov 23 19:24:07 CET 2012)
 * MAFalda Author:  John Idarraga <idarraga@cern.ch>
 *
 * An example of how to use the clustering results
 * for further processing.
 */

#ifndef __RelaxPions_h
#define __RelaxPions_h

#include "MPXAlgo/MediPixAlgo.h"
#include "MPXAlgo/Signals.h"
#include "MPXAlgo/Highlighter.h"

#include "CalibrationLoader/CalibrationLoader.h"

// This algorithm is using an object put in the StoreGate
//  by BlobsFinder. I need to know the object.
#include "BlobsFinder/BlobsFinder.h"

class RelaxPions : public MediPixAlgo , public CalibrationLoader {

public:

  RelaxPions();
  virtual ~RelaxPions() { };

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();

private:

  AllBlobsContainer * m_aB;
  Int_t m_minNPixels;

  // config
  int m_sizex;
  int m_sizey;

  // for output
  vector<int> m_clusterTOT;
  vector<double> m_clusterEnergy;
  vector<int> m_clusterSize;
  vector<int> m_clusterAngle;

  int m_nGoodCandidates;
  int m_nGoodCandidatesFragmented;
  vector<int> m_GoodCadidatesClusterSize;

  // Number of entries histo
  TH2I * m_globalHitsEntries;
  TH2D * m_globalAverageTOT;

  ClassDef(RelaxPions, 1)
};

#endif

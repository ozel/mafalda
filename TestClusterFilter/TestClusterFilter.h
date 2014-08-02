
/**
 * Created automatically with MAFalda (Mi 30 Jul 2014 18:02:57 CEST)
 * MAFalda Author:  John Idarraga <idarraga@cern.ch>
 *
 * An example of how to use the clustering results
 * for further processing.
 */

#ifndef __TestClusterFilter_h
#define __TestClusterFilter_h

#include "MPXAlgo/MediPixAlgo.h"
#include "CalibrationLoader/CalibrationLoader.h"

// This algorithm is using an object put in the StoreGate
//  by BlobsFinder. I need to know the object.
#include "BlobsFinder/BlobsFinder.h"

class TestClusterFilter : public MediPixAlgo , public CalibrationLoader {

public:

  TestClusterFilter();
  virtual ~TestClusterFilter() { };

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();

private:

  AllBlobsContainer * m_aB;
  Int_t m_minNPixels;

  // for output
  vector<int> m_clusterTOT;
  vector<int> m_hotestPixel;
  vector<double> m_clusterEnergy;

  ClassDef(TestClusterFilter, 1)
};

#endif

#ifndef AfterClusteringExample_h
#define AfterClusteringExample_h

#include "MPXAlgo/MediPixAlgo.h"

// This algorithm is using an object put in the StoreGate
//  by BlobsFinder. I need to know the object.
#include "BlobsFinder/BlobsFinder.h"

class AfterClusteringExample : public MediPixAlgo {

public:

  AfterClusteringExample();
  virtual ~AfterClusteringExample() { };

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
  vector<Float_t> m_perpdistance;

  ClassDef(AfterClusteringExample, 1)
};

#endif

#ifndef __EffiencyMap_h
#define __EffiencyMap_h

#include "MPXAlgo/MediPixAlgo.h"

// This algorithm is using an object put in the StoreGate
//  by BlobsFinder. I need to know the object.
#include "BlobsFinder/BlobsFinder.h"

class EffiencyMap : public MediPixAlgo {

public:

  EffiencyMap();
  virtual ~EffiencyMap() { };

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();

private:

  AllBlobsContainer * m_aB;
  Int_t m_maxNHits;
  bool m_firstTimeFlag;

  TH2F * m_hitMap;
  TH2F * m_equalizationMap;
  vector<vector<int> > m_perPixelTOT;

  ClassDef(EffiencyMap, 1)
};

#endif

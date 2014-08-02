#ifndef Bfieldtest_h
#define Bfieldtest_h

#include "MPXAlgo/MediPixAlgo.h"

// This algorithm is using an object put in the StoreGate
//  by BlobsFinder. I need to know the object.
#include "BlobsFinder/BlobsFinder.h"

class Bfieldtest : public MediPixAlgo {

public:

  Bfieldtest();
  virtual ~Bfieldtest() { };

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
  vector<Double_t> m_deviation_sh;
  vector<Double_t> m_deviation_dh;

  typedef struct {
	  Double_t x;
	  Double_t y;
  } VertexInfo;
  VertexInfo m_vertex;

  ClassDef(Bfieldtest, 1)
};

#endif

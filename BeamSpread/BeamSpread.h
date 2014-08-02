/**
 * Author John Idarraga <idarraga@lal.in2p3.fr>
 * 2011
 */

#ifndef BeamSpread_h
#define BeamSpread_h

#include "MPXAlgo/MediPixAlgo.h"

class AllBlobsContainer;

class BeamSpread : public MediPixAlgo {

public:

  BeamSpread();
  virtual ~BeamSpread() { };

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();

private:

  AllBlobsContainer * m_allBlobs;
  Double_t m_spreadDist;
  Double_t m_pixelSizeX;
  Double_t m_pixelSizeY;

  ClassDef(BeamSpread, 1)
};

#endif

#ifndef __OctoAlignment_h
#define __OctoAlignment_h

#include "MPXAlgo/MediPixAlgo.h"

// This algorithm is using an object put in the StoreGate
//  by BlobsFinder. I need to know the object.
#include "BlobsFinder/BlobsFinder.h"
#include "OctoCEA/OctoCEA.h"

class OctoAlignment : public MediPixAlgo {

public:

  OctoAlignment();
  virtual ~OctoAlignment() { };

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();

private:

  // Variables for output
  vector<double> m_angleDifferences1_2;
  vector<double> m_angleDifferences2_3;
  vector<double> m_angleDifferences3_4;
  vector<double> m_angleDifferences1_3;
  vector<double> m_angleDifferences2_4;
  vector<double> m_angleDifferences1_4;
  vector<double> m_cutDifferences1_2;
  vector<double> m_cutDifferences2_3;
  vector<double> m_cutDifferences3_4;
  vector<double> m_cutDifferences1_3;
  vector<double> m_cutDifferences2_4;
  vector<double> m_cutDifferences1_4;

  // Containers to retrieve from StoreGate
  AllBlobsContainer * m_clusterContainerPtr;
  OctoTracksContainer * m_candidatesContainerPtr;

  ClassDef(OctoAlignment, 1)
};

#endif

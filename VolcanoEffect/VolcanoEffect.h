#ifndef __VolcanoEffect_h
#define __VolcanoEffect_h

#include "MPXAlgo/MediPixAlgo.h"

// This algorithm is using an object put in the StoreGate
//  by BlobsFinder. I need to know the object.
#include "BlobsFinder/BlobsFinder.h"

class VolcanoEffect : public MediPixAlgo {

public:

  VolcanoEffect();
  virtual ~VolcanoEffect();

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();

  unsigned int CalculateSquareShapeSignature(blob);
  unsigned int CalculateSquareShapeSignature(int, int);

  void PrintMeshSeries(int, int);
  int GetNewMeshSize(int, int);

  void StartAverageClusterMatrix();
  void AppendtoAverageClusterMatrix(TGraph2D * g);
  void NormalizeAverageClusterMatrix();
  TGraph2D * ConvertClusterMatrixToTGraph2D();

  TH1 * ExtractSlice(TString);

private:

  AllBlobsContainer * m_aB;
  int m_meshdiv;
  int m_averageCluster_x;
  int m_averageCluster_y;
#ifndef __CINT__ // let's hide this from CINT
  double ** m_averageClusterMatrix;
#endif
  int m_averageClusterEntries;
  unsigned int m_averageClusterUniqueSignature;
  TGraph2D * m_averagaCluster;
  vector<TH1 *> m_averageClusterSlice;
  int m_frameId;

  TCanvas * m_canvas_g2;
  TCanvas * m_canvas_p1;
  TCanvas * m_canvas_p2;

  // for output
  vector<unsigned int> m_squaresignature;
  vector<double> m_clusterEnergy;

  ClassDef(VolcanoEffect, 1)
};

#endif

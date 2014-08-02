/**
 * Author: John Idarraga <idarraga@cern.ch> , 2008
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef AnalysisExample_h
#define AnalysisExample_h

#include "MPXAlgo/MediPixAlgo.h"
#include "MPXAlgo/Highlighter.h"

class AnalysisExample : public MediPixAlgo {

public:

  AnalysisExample();
  virtual ~AnalysisExample() { };

  // You have to implement Init(), Execute() and Finalize()
  //  if you inherit from MediPixAlgo
  void Init();
  void Execute();
  void Finalize();

  Bool_t ThereIsSomething(Int_t, Int_t);

private:

  Int_t m_totalCharge;
  Int_t m_totalHits;
  Int_t m_xcoor;
  Int_t m_ycoor;

  Highlighter * m_arrow;

  ClassDef(AnalysisExample, 1)
};

#endif

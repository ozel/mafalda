#ifndef MetaData_h
#define MetaData_h

#include "MPXAlgo/MediPixAlgo.h"

class MetaData : public MediPixAlgo {

public:

  MetaData();
  virtual ~MetaData() { };

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();

private:

  Int_t m_fId;
  Float_t m_acqTime;
  Double_t m_startTime;
  TString m_startTimeS;
  Double_t m_HV;

  ClassDef(MetaData, 1)
};

#endif

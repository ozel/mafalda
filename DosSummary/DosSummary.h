#ifndef __DosSummary_h
#define __DosSummary_h

#include "MPXAlgo/MediPixAlgo.h"

#include "DosSummaryReport.h"
#include "BlobsFinder/BlobsFinder.h"

class DosSummary : public MediPixAlgo {

public:

  DosSummary();
  virtual ~DosSummary() { };

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();

private:

  AllBlobsContainer * m_aB;

  DosSummaryReport m_dos_summary_report;

  ClassDef(DosSummary, 1)
};

#endif

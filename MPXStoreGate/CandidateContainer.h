/**
 * Author: John Idarraga <idarraga@cern.ch> , 2009
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef CandidateContainer_h
#define CandidateContainer_h

#include <vector>

#include <TROOT.h>
#include <TObject.h>
#include <TString.h>

#include "AnalysisCore/AnalysisCore_defs.h"

using namespace std;

class MediPixAlgo;

/** 
 * Class holding the information about each candidate 
 *
 */

class CandidateContainer : public TObject {

 public:
  CandidateContainer();
  CandidateContainer(MediPixAlgo *);
  CandidateContainer(MediPixAlgo *, MPXDefs::SpecialObjs);
  virtual ~CandidateContainer(){};

  void SetAuthor(TString);
  void SetDSN(TString);
  void SetnFrame(Int_t);
  void SetLikelihood(Double_t);
  
  inline TString GetAuthor(){
    return author;
  };
  inline TString GetDSN(){
    return DSN;
  };
  inline Int_t GetnFrame(){
    return nFrame;
  };
  inline Double_t GetLikelihood(Int_t pos){
    return Likelihood[pos];
  };
  inline Int_t GetFrameXSize(){return m_xFrameSize;};
  inline Int_t GetFrameYSize(){return m_yFrameSize;};
  inline MPXDefs::SpecialObjs GetType(){return m_type;};

 private:
  TString author;
  TString DSN;
  Int_t m_xFrameSize;
  Int_t m_yFrameSize;
  Int_t nFrame;
  vector<Double_t> Likelihood;

  MPXDefs::SpecialObjs m_type; // for special objects
 
  ClassDef(CandidateContainer, 1)
};

#endif

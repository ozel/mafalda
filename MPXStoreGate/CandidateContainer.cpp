/**
 * Author: John Idarraga <idarraga@cern.ch> , 2009
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef CandidateContainer_cpp
#define CandidateContainer_cpp

#include "CandidateContainer.h"
#include "MPXAlgo/MediPixAlgo.h"

ClassImp(CandidateContainer)

CandidateContainer::CandidateContainer()
  : TObject() {
}

CandidateContainer::CandidateContainer(MediPixAlgo * algo) 
  : TObject() {

  author = algo->GetAlgoName();
  //DSN = algo->GetDataSetNumber();
  nFrame = algo->GetFrameId();
  m_xFrameSize = algo->GetMatrixXdim();
  m_yFrameSize = algo->GetMatrixYdim();
  m_type = MPXDefs::REGULAR; // not an special object

}

CandidateContainer::CandidateContainer(MediPixAlgo * algo,
				       MPXDefs::SpecialObjs type)
  : TObject() {
  
  //CandidateContainer(algo);
  author = algo->GetAlgoName();
  //DSN = algo->GetDataSetNumber();
  nFrame = algo->GetFrameId();
  m_xFrameSize = algo->GetMatrixXdim();
  m_yFrameSize = algo->GetMatrixYdim();
  m_type = type; // some special object, see MPXDefs::SpecialObjs

}

void CandidateContainer::SetAuthor(TString author_i){
  author = author_i;
}

void CandidateContainer::SetDSN(TString DSN){
  DSN=DSN;
}

void CandidateContainer::SetnFrame(Int_t nFrame){
  nFrame=nFrame;
}

void CandidateContainer::SetLikelihood(Double_t element){
  Likelihood.push_back(element);
}


#endif

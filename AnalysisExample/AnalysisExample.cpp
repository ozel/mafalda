/**
 * Author: John Idarraga <idarraga@cern.ch> , 2008
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef AnalysisExample_cpp
#define AnalysisExample_cpp

#include "AnalysisExample.h"
#include "TF1.h"

using namespace MSG;

ClassImp(AnalysisExample)

AnalysisExample::AnalysisExample(){

  m_totalCharge = 0;
  m_totalHits = 0;

}

void AnalysisExample::Init(){

  // You will get an ntuple file containing a TTree with the name of the
  //  Algorithm.  The branches registered through getMyTree() get writte
  //  in that tree and you can fill them each time Execute() gets called.
  getMyTree()->Branch("totalCharge", &m_totalCharge , "totalCharge/I");

  m_xcoor = 0;
  m_ycoor = 0;

  RegisterConfigurationValue(&m_xcoor, "xcoor");
  RegisterConfigurationValue(&m_ycoor, "ycoor");

  Log << MSG::INFO << "Init function !" << endreq;

  //TF1 * f1 = new TF1("hola","x",0,1.);
  //f1->GetExpFormula();

}

void AnalysisExample::Execute(){

  // REMINDER: AnalysisExample::Execute() runs once per frame
  //  you may need ro reinitialize variables.
  m_totalCharge = 0;
  m_totalHits = 0;
  
  Int_t xDim = GetMatrixXdim();
  Int_t yDim = GetMatrixYdim();

  for(Int_t colItr = 0; colItr < xDim-1 ; colItr++)
    {
      for(Int_t rowItr = 0; rowItr < yDim-1 ; rowItr++)
	{
	  if(ThereIsSomething(colItr, rowItr))
	    {
	      m_totalCharge += GetMatrixElement(colItr, rowItr);
	      m_totalHits++;
	    }
	}
    }
  Log << MSG::INFO << "Total charge : " << m_totalCharge << endreq ;
  Log << MSG::INFO << "Total hits : " << m_totalHits << endreq ;

  // Draw an arrow if using MPXViewer
  m_arrow = new Highlighter(m_xcoor, m_ycoor, "arrow", this);
  PullToStoreGateAccess(m_arrow, MPXDefs::DO_NOT_SERIALIZE_ME);

  // fill the Tree
  getMyTree()->Fill();

}

Bool_t AnalysisExample::ThereIsSomething(Int_t i, Int_t j){

  if(GetMatrixElement(i, j) > 0)
    return true;
  else
    return false;

  return false;
}

void AnalysisExample::Finalize(){

  Log << MSG::INFO << "Finalize function !" << endreq;

}

#endif

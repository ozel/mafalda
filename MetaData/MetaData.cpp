#ifndef MetaData_cpp
#define MetaData_cpp

#include "MetaData.h"

using namespace MSG;

ClassImp(MetaData)

MetaData::MetaData(){
  
}

void MetaData::Init(){

  // You will get an ntuple file containing a TTree with the name of this
  //  Algorithm.  The branches registered through getMyTree() get registered
  //  in that tree so you can fill them each time Execute() gets called.
  getMyTree()->Branch("fId", &m_fId , "fId/I");
  getMyTree()->Branch("acqTime", &m_acqTime , "acqTime/F");
  getMyTree()->Branch("startTime", &m_startTime , "startTime/D");
  getMyTree()->Branch("startTimeS", &m_startTimeS);
  getMyTree()->Branch("biasVoltage", &m_HV, "biasVoltage/D");
}

void MetaData::Execute(){

  m_fId = GetFrameId();
  m_acqTime = GetAcqTime();
  m_startTime = GetStartTime();
  m_startTimeS = GetStartTimeS();
  m_HV = GetHV();

  getMyTree()->Fill();

}

void MetaData::Finalize(){


}

#endif

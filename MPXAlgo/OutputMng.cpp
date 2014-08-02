/**
 * Author: John Idarraga <idarraga@cern.ch> , 2009
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef OutputMng_cpp
#define OutputMng_cpp

#include <iostream>
#include <TROOT.h>
#include <TString.h>
#include "OutputMng.h"

ClassImp(OutputMng)

OutputMng::OutputMng(){
  OutputLevel = MSG::INFO;
  newOutputLine = 1;
  newAlgoIndicator = 1;
  std::cout.precision(5);
}

void OutputMng::setAlgoName(TString algoName_i){
  algoName = algoName_i;
}

Bool_t OutputMng::isActive(){
  if(requestedLevel >= OutputLevel)
    return true;
  return false;
}

OutputMng & OutputMng::operator<<(MSG::Level inLevel){
  requestedLevel = inLevel;
  newOutputLine = 1;
  return *this;
}

//OutputMng & OutputMng::operator<<(MSG::Endreq endreq){
OutputMng & OutputMng::operator<<(MSG::Endreq){
  this->newAlgoIndicator = 1;
  if(this->isActive())
    {
      std::cout << std::endl;
    }
  return *this;
}

/*
OutputMng & OutputMng::operator<<(Char_t * st){
  if(OutputLevel == requestedLevel)
    std::cout << st;
  return *this;
}
OutputMng & OutputMng::operator<<(Int_t st){
  if(OutputLevel == requestedLevel)
    std::cout << st;
  return *this;
}
*/

/*
OutputMng &  OutputMng::operator<<(std::ios_base& (*_f)(std::ios_base&)){
  if(isActive())
    std::cout << "|-4-|";
     return *this;
}

OutputMng & OutputMng::operator<<(std::ios& (*_f)(std::ios&)){
  if(isActive())
    std::cout << "|-3-|";
  return *this;
}

OutputMng & OutputMng::operator<<(std::ostream& (*_f)(std::ostream&))    {
  if(isActive())
    std::cout << "|-2-|";
  return *this;
}
*/

//OutputMng & OutputMng::operator<<(OutputMng& (*_f)(OutputMng&)){
OutputMng & OutputMng::operator<<(OutputMng& (*)(OutputMng&)){
  if(isActive())
    std::cout << std::endl;
  return *this;
}

#endif

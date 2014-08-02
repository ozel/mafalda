/**
 * Author: John Idarraga <idarraga@cern.ch> , 2008
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef ConfigurationValue_h
#define ConfigurationValue_h

#include <string>

#include <TROOT.h>

#include "AnalysisCore/AnalysisCore_defs.h"
#include "MPXStoreGate/CandidateContainer.h"

using namespace std;

class MediPixAlgo;

/**
 * This class handles values to visualize and modify from the MediPix
 *  viewer.  Note the hyphen in the definitions in LinkDef.h
 *
 */

template<class T> 
class ConfigurationValue : public CandidateContainer {

 private:
  string m_valuesConfName;
  T * m_valuesConfVal; // pointer to actual values in the Algorithm
  
 public:
  ConfigurationValue();
  ConfigurationValue(MediPixAlgo *, T *, 
		     const char *, 
		     MPXDefs::SpecialObjs);
  ~ConfigurationValue();  // non need for virtual constructor, child class.
  
  T GetConfigValue(){ return * m_valuesConfVal; };
  string GetConfigName(){ return m_valuesConfName; };

  void SetConfigValue(T val);
  
  ClassDef(ConfigurationValue, 1)
};

#endif

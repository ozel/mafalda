/**
 * Author: John Idarraga <idarraga@cern.ch> , 2008
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef ConfigurationValue_cpp
#define ConfigurationValue_cpp

#include "ConfigurationValue.h"

templateClassImp(ConfigurationValue)

/**
 * This class handles values to visualize/tweak from the MediPix
 *  viewer.
 *
 */

// default constructor
template<class T> 
ConfigurationValue<T>::ConfigurationValue()
  : CandidateContainer() {
}

template<class T> 
ConfigurationValue<T>::ConfigurationValue(MediPixAlgo * algo, T * val, 
					  const char * valname, 
					  MPXDefs::SpecialObjs objType) 
  : CandidateContainer(algo, objType) {
  
  m_valuesConfName = valname;

  //m_valuesConfVal = new T; // <<----- check this out !!
  m_valuesConfVal = val;

}

template<class T> 
ConfigurationValue<T>::~ConfigurationValue()
{
  delete m_valuesConfVal;
}

template<class T> 
void ConfigurationValue<T>::SetConfigValue(T val){ 
  *m_valuesConfVal = val; 
}

// In a shared lib scenario (or if you define the template in a different file, like here) 
//  you have to tell what are you going to instantiate
template class ConfigurationValue<Int_t>;
template class ConfigurationValue<Float_t>;
template class ConfigurationValue<Double_t>;
template class ConfigurationValue<Bool_t>;

#endif

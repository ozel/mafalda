/**
 * Author: John Idarraga <idarraga@cern.ch> , 2009
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef Signals_cxx
#define Signals_cxx

#include <typeinfo>

#include "Signals.h"

ClassImp(Signals)


/**
 * Class for drawing Signalss in the Viewer.
 * I will may be move this somewhere else later. 
 *
 */

Signals::Signals () : CandidateContainer () {

}

Signals::Signals (MediPixAlgo * algo, signal_type) : CandidateContainer(algo, MPXDefs::VIS_SKIP) {

}

#endif

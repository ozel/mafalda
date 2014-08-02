/**
 * Author: John Idarraga <idarraga@cern.ch> , 2009
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef Signals_h
#define Signals_h

#include <vector>

#include <TROOT.h>
#include <TArrow.h>
#include <TEllipse.h>
#include <TBrowser.h>
#include <TEllipse.h>
#include <TString.h>
#include <TMath.h>
#include <TLine.h>

#include "MPXAlgo/MediPixAlgo.h"
#include "MPXStoreGate/MPXStoreGate.h"
#include "MPXStoreGate/CandidateContainer.h"

#include "OutputMng.h"

using namespace std;

/**
 * Class for drawing Signalss in the Viewer.
 * I will may be move this somewhere else later. 
 *
 */

class MAFTArrow;
class MAFTEllipse;
class MAFTLine;


class Signals : public CandidateContainer {

public:

	typedef enum {
		__SIGNAL_SKIP = 0
	} signal_type;

	Signals();
	Signals(MediPixAlgo *, signal_type);
	~Signals(){};

	OutputMng Log;
	MSG::Endreq endreq;


	ClassDef(Signals, 0)
};


#endif

/**
 * Author: John Idarraga <idarraga@cern.ch> , 2009
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef OutputMng_h
#define OutputMng_h

#include <iostream>
#include "AnalysisCore/AnalysisCore_defs.h"

/**
 * This class manages the output level for MPXAlgos
 *
 */

class OutputMng {

public:
	OutputMng();
	virtual ~OutputMng() { };

	OutputMng & operator<<(MSG::Level);
	OutputMng & operator<<(MSG::Endreq);
	OutputMng & operator<<(OutputMng& (*)(OutputMng&));
	//OutputMng & operator<<(std::ostream& (*) (std::ostream&));
	//OutputMng & operator<<(std::ios& (*)(std::ios&));
	//OutputMng & operator<<(std::ios_base& (*)(std::ios_base&));

	void setAlgoName(TString);
	Bool_t isActive();

	MSG::Level OutputLevel;
	MSG::Level requestedLevel;

	Int_t newOutputLine;
	Int_t newAlgoIndicator;
	TString algoName;

	ClassDef(OutputMng, 1)
};

// non member functions needed

inline void printBlank(TString nameA){

	for(Int_t spc = 0 ; spc < LEFT_MSG_TAB - nameA.Sizeof() ;spc++)
	{
		std::cout << " ";
	}
}

template <typename T>
OutputMng & operator << (OutputMng& lhs, const T& arg)  {

	//std::cout.precision(5);

	if(lhs.isActive())
	{
		if(lhs.newOutputLine)
		{
			if(lhs.newAlgoIndicator)
			{
				std::cout << "["<< lhs.algoName << "] ";
				printBlank(lhs.algoName);
				lhs.newAlgoIndicator = 0;
			}
			else
			{

			}
			lhs.newOutputLine = 0;
		}
		std::cout << arg;
	}
	return lhs;
}


#endif

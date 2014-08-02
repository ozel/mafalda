#ifndef PPSResidual_h
#define PPSResidual_h

#include "MPXAlgo/MediPixAlgo.h"

class AllBlobsContainer;

class PPSResidual : public MediPixAlgo {

public:

	PPSResidual();
	virtual ~PPSResidual() { };

	// You ought to implement Init(), Execute() and Finalize()
	//  when you inherit from MediPixAlgo.  This model gives you
	//  direct access to data and services.
	void Init();
	void Execute();
	void Finalize();
	void SetTilted(bool f){m_tilted = f;};

private:

	AllBlobsContainer * m_allBlobs;

	Double_t m_pixelSizeX;
	Double_t m_pixelSizeY;
	bool m_tilted;

	// for output
	vector<Double_t> m_deviation_sh;
	vector<Double_t> m_deviation_dh;
	Int_t m_nClusters;

	typedef struct {
		Double_t x;
		Double_t y;
	} VertexInfo;
	VertexInfo m_vertex;

	ClassDef(PPSResidual, 1)
};

#endif

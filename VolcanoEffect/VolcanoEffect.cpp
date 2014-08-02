/**
 * Author:  John Idarraga <idarraga@cern.ch> , 2011
 *  An example of how to use the Clustering results
 *  for further processing.
 */

#ifndef __VolcanoEffect_cpp
#define __VolcanoEffect_cpp

#include "VolcanoEffect.h"
#include "MAFTools.h"

using namespace MSG;

ClassImp(VolcanoEffect)

VolcanoEffect::VolcanoEffect(){

	m_meshdiv = 1; // default mesh division = 2Sx + 1, 2Sy + 1
	m_canvas_g2 = 0x0;
	m_canvas_p1 = 0x0;
	m_canvas_p2 = 0x0;

	// size of the average cluster to store
#define __size_average_x 7
#define __size_average_y 7
	//#define __size_average_x 7 // Cluster 7x7 --> identifier 0x0707 = 1799
	//#define __size_average_y 7

	m_averageClusterUniqueSignature = CalculateSquareShapeSignature(__size_average_x, __size_average_y);

	m_averageCluster_x = GetNewMeshSize(__size_average_x, m_meshdiv); // Cluster 7x7 --> identifier 0x0707 = 1799
	m_averageCluster_y = GetNewMeshSize(__size_average_y, m_meshdiv);
	m_averageClusterEntries = 0;

}

VolcanoEffect::~VolcanoEffect(){


	// Delete average cluster matrix
	for(int i = 0 ; i < m_averageCluster_x ; i++) {
		delete [] m_averageClusterMatrix[i];
	}
	delete [] m_averageClusterMatrix;

}

void VolcanoEffect::Init(){

	Log << MSG::INFO << "Init function !" << endreq;

	// This value will be overridden by the configuration since it'll be set up
	//  a few lines below as a configuration value

	// You will get an ntuple file containing a TTree with the name of this
	//  Algorithm.  The branches registered through getMyTree() get registered
	//  in that tree so you can fill them each time Execute() gets called.
	getMyTree()->Branch("squareSignature", &m_squaresignature);
	getMyTree()->Branch("clusterEnergy", &m_clusterEnergy);
	getMyTree()->Branch("frameId", &m_frameId, "frameId/I");

	// A configuration value that can be tuned from the Viewer
	RegisterConfigurationValue(&m_meshdiv, "meshdiv");
	RegisterConfigurationValue(&m_averageCluster_x, "averageCluster_x");
	RegisterConfigurationValue(&m_averageCluster_y, "averageCluster_y");

	// Initialize cluster average matrix
	StartAverageClusterMatrix();

}

void VolcanoEffect::Execute(){

	m_frameId = GetFrameId();

	if(m_canvas_g2) { delete m_canvas_g2; }
	if(m_canvas_p1) { delete m_canvas_p1; }
	if(m_canvas_p2) { delete m_canvas_p2; }

	// Ask the store gate if the previous algorithm (BlobsFinder --> reponsible for clustering)
	//  sent any objects to the StoreGate.
	Int_t lastObject = GetNumberOfObjectsWithAuthor("BlobsFinder");
	if(lastObject == 0)
		return;

	// If so, get the pointer to the last object.  BlobsFinder, as it comes out of the box, sends
	//  a single object containing all clusters.
	m_aB = (AllBlobsContainer *) GetObjectFromAuthor("BlobsFinder", lastObject-1);

	// AllBlobsContainer is a box full of Blobs(Clusters). Now we can iterate over all clusters inside.
	vector<blob> blobsVector = m_aB->GetBlobsVector();
	Log << MSG::INFO << "Number of blobs from clustering = " << (Int_t) blobsVector.size() << endreq;
	vector<blob>::iterator blobsItr = blobsVector.begin(); //allBlobs.begin();


	vector<TGraph2D *> vg;
	vector<TH1 *> vh_x;
	vector<TH1 *> vh_y;

	blob b;
	int cntr_x = 0;
	int cntr_y = 0;

	bool done = false;

	for ( ; blobsItr != blobsVector.end() ; blobsItr++) {

		b = *blobsItr;

		if(b.bP.nInnerPixels < 10) continue;


		m_clusterEnergy.push_back(b.bP.clusterEnergy);

		Log << MSG::DEBUG << "Cluster Energy = " << b.bP.clusterEnergy << endreq;

		// Square shape signature
		unsigned int sign = CalculateSquareShapeSignature(*blobsItr);
		m_squaresignature.push_back( sign );

		Log << MSG::DEBUG << "Cluster : " << sign << " | " << m_averageClusterUniqueSignature << " --> width_x = " << b.bP.width_x << ", width_y = " << b.bP.width_y << endreq;

		//if(b.bP.geoCenter_x >= 200 && b.bP.geoCenter_x <= 240 && b.bP.geoCenter_y >= 170 && b.bP.geoCenter_y <= 190) {
		//if(sign == m_averageClusterUniqueSignature) { // Example: 1799 = 7x7 --> unique signature, all cluster of same width_x X width_y
			//Log << MSG::INFO << "Pre-selected cluster !" << endreq;

		if(!done){

			TGraph2D * g = MAFTools::ConvertClusterToGraph2D(*blobsItr, m_meshdiv, GetFrameId(), "E");
			vg.push_back(g);

			blob b_ext = MAFTools::BuildBlob(g);
			b_ext.CalculateProperties(1E4, 1E4, 1.0);
			TH1 * h_x = MAFTools::ExtractSlice(b_ext, cntr_x, "X");
			TH1 * h_y = MAFTools::ExtractSlice(b_ext, cntr_y, "Y");

			vector<double> * criticalPoints;
			//int npeaksx = MAFTools::IdendifyPeaks_A1( h_x , criticalPoints );
			//int npeaksy = MAFTools::IdendifyPeaks_A1( h_y , criticalPoints );

			//Log << MSG::DEBUG << "Slice [" << cntr_x << "]" << " npeaks = " << npeaksx << endreq;
			//Log << MSG::DEBUG << "Slice [" << cntr_y << "]" << " npeaks = " << npeaksy << endreq;

			vh_x.push_back( h_x );
			vh_y.push_back( h_y );

			//if(b.bP.clusterEnergy > 9000. && b.bP.clusterEnergy < 10000.) { // keV --> strange clusters with energy loss (volcano effect)
			//if(b.bP.clusterEnergy > 11000. && b.bP.clusterEnergy < 13500.) { // keV --> strange clusters with energy loss (volcano effect)
				//Log << MSG::INFO << "Averageable cluster !" << endreq;
			//MAFTools::
			AppendtoAverageClusterMatrix(g);

			done = true;
		}

			//}
		//}
		cntr_x++;
		cntr_y++;
	}

	if(!vg.empty()) m_canvas_g2 = DrawInSeparateWindow(vg);
	if(!vh_x.empty()) m_canvas_p1 = DrawInSeparateWindow(vh_x);
	if(!vh_y.empty()) m_canvas_p2 = DrawInSeparateWindow(vh_y);

	//PrintMeshSeries(5, 10);

	// Fill the output tree of this algorithm
	getMyTree()->Fill();

	// WARNING ! don't forget to clean up your variables for the next TTree::Fill call
	m_squaresignature.clear();
	m_clusterEnergy.clear();
}

void VolcanoEffect::PrintMeshSeries(int S, int maxN){

	for(int N = 0 ; N < maxN ; N++){

		int newsize = TMath::Power(2, N) * S;
		for(int n = 0 ; n <= N-1 ; n++){
			newsize -= TMath::Power(2, n);
		}

		Log << MSG::DEBUG << "N = " << N << " --> " << newsize << endreq;

	}
}

int VolcanoEffect::GetNewMeshSize(int S, int N){

	int newsize = TMath::Power(2, N) * S;
	for(int n = 0 ; n <= N-1 ; n++){
		newsize -= TMath::Power(2, n);
	}
	return newsize;
}

unsigned int VolcanoEffect::CalculateSquareShapeSignature(blob b){

	return CalculateSquareShapeSignature(b.bP.width_x, b.bP.width_y);
}

unsigned int VolcanoEffect::CalculateSquareShapeSignature(int x, int y){

	unsigned int sign = 0;
	// Take the two 32 bits integers b.bP.width_x, b.bP.width_y
	// which won't be bigger than 256 (8bits).
	sign |= x;
	// shift it 8 bits
	sign <<= 8;
	// and OR the width_x part
	sign |= y;

	// sign is now unique for the pair b.bP.width_x, b.bP.width_y
	return sign;
}


void VolcanoEffect::StartAverageClusterMatrix(	){

	m_averageClusterMatrix = new double * [m_averageCluster_x];
	for(int i = 0 ; i < m_averageCluster_x ; i++){
		m_averageClusterMatrix[i] = new double[m_averageCluster_y];
	}

	// zeeero it
	for(int i = 0 ; i < m_averageCluster_x ; i++){
		for(int j = 0 ; j < m_averageCluster_y ; j++){
			m_averageClusterMatrix[i][j] = 0;
		}
	}

}
void VolcanoEffect::AppendtoAverageClusterMatrix(TGraph2D * g){

	int Np = g->GetN();
	double * xarr = g->GetX();
	double * yarr = g->GetY();
	double * zarr = g->GetZ();

	// Dumb dimensions check, this may fail.  But at this point I should not
	// be able to blow it.
	Log << MSG::DEBUG << "Np = " << Np << " | " << m_averageCluster_x << "," << m_averageCluster_y << endreq;
	if ( Np != m_averageCluster_x * m_averageCluster_y ){
		Log << MSG::ERROR << "Can not use this cluster for average " << endreq;
		return;
	}
	// at this point. This should populate the whole matrix;
	//Log << MSG::DEBUG << "Adding cluster : " << endreq;
	for (int i = 0 ; i < Np ; i++) {
		//Log << MSG::DEBUG << (int)xarr[i] << " , " << (int)yarr[i] << " : " << (int)zarr[i] << endreq;
		m_averageClusterMatrix[ (int)xarr[i] ][ (int)yarr[i] ] += zarr[i];
	}
	m_averageClusterEntries++;

}
void VolcanoEffect::NormalizeAverageClusterMatrix(){

	//MAFTools::PrintMatrix(m_averageClusterMatrix, m_averageCluster_x, m_averageCluster_y);
	Log << MSG::INFO << "The number of selected cluster for average is " << m_averageClusterEntries << endreq;
	for(int i = 0 ; i < m_averageCluster_x ; i++){
		for(int j = 0 ; j < m_averageCluster_y ; j++){
			m_averageClusterMatrix[i][j] /= m_averageClusterEntries;
		}
	}

}

TGraph2D * VolcanoEffect::ConvertClusterMatrixToTGraph2D(){

	int N = m_averageCluster_x * m_averageCluster_y;
	double * xarr = new double[N];
	double * yarr = new double[N];
	double * zarr = new double[N];

	for(int i = 0 ; i < m_averageCluster_x ; i++){
		for(int j = 0 ; j < m_averageCluster_y ; j++){
			xarr[j*m_averageCluster_x + i] = i;
			yarr[j*m_averageCluster_x + i] = j;
			zarr[j*m_averageCluster_x + i] = m_averageClusterMatrix[i][j];
		}
	}

	TGraph2D * g = new TGraph2D(N, xarr, yarr, zarr);
	g->SetName("AverageCluster");

	return g;
}

TH1 * VolcanoEffect::ExtractSlice(TString proj){

	int sIndx_x = TMath::FloorNint( m_averageCluster_x/2 );
	int sIndx_y = TMath::FloorNint( m_averageCluster_y/2 );

	TString name = "slice_";
	name += proj;
	TH1F * h;

	if (proj == "y") {

		Log << MSG::DEBUG << "Get slice along " << proj << " at x = " << sIndx_x << endreq;
		h = new TH1F(name, name, m_averageCluster_y, 0, m_averageCluster_y);
		int i = sIndx_x;
		for(int j = 0 ; j < m_averageCluster_y ; j++){
			h->Fill(j, m_averageClusterMatrix[i][j]);
		}

	} else if (proj == "x") {

		Log << MSG::DEBUG << "Get slice along " << proj << " at y = " << sIndx_y << endreq;
		h = new TH1F(name, name, m_averageCluster_x, 0, m_averageCluster_x);
		int j = sIndx_y;
		for(int i = 0 ; i < m_averageCluster_x ; i++){
			h->Fill(i, m_averageClusterMatrix[i][j]);
		}

	}

	return static_cast<TH1*>(h);
}

void VolcanoEffect::Finalize() {

	// Normalize
	NormalizeAverageClusterMatrix();
	// Convert to TGraph2D
	m_averagaCluster = ConvertClusterMatrixToTGraph2D();
	// Store to Output file
	m_averagaCluster->Write();

	// slices
	vector<TH1 *> h_slices;
	TH1 * h_xslice = ExtractSlice("x");
	h_slices.push_back(h_xslice); h_xslice->Write();
	TH1 * h_yslice = ExtractSlice("y");
	h_slices.push_back(h_yslice); h_yslice->Write();

	DrawInSeparateWindow(h_slices, MSG::DEBUG);

	Log << MSG::INFO << "Finalize function !" << endreq;

}

#endif

/**
 * Author:  John Idarraga <idarraga@cern.ch> , 2011
 *  An example of how to use the Clustering results
 *  for further processing.
 */

#ifndef Bfieldtest_cpp
#define Bfieldtest_cpp

#include "Bfieldtest.h"
#include "MAFTools.h"

using namespace MSG;

ClassImp(Bfieldtest)

Bfieldtest::Bfieldtest(){

}

void Bfieldtest::Init(){

	Log << MSG::INFO << "Init function !" << endreq;

	// This value will be overridden by the configuration since it'll be set up
	//  a few lines below as a configuration value
	m_minNPixels = 5;

	// You will get an ntuple file containing a TTree with the name of this
	//  Algorithm.  The branches registered through getMyTree() get registered
	//  in that tree so you can fill them each time Execute() gets called.
	getMyTree()->Branch("deviation_single", &m_deviation_sh);
	getMyTree()->Branch("deviation_double", &m_deviation_dh);

	getMyTree()->Branch("vertex", &m_vertex.x, "vertexX/D:vertexY");

	// A configuration value that can be tuned from the Viewer
	RegisterConfigurationValue(&m_minNPixels, "minNPixels");

}

void Bfieldtest::Execute(){

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

	for( ; blobsItr != blobsVector.end() ; blobsItr++)
	{

		blobtype bt = (*blobsItr).GetBlobType();
		Log << MSG::INFO << "blobtype is: " << bt << endreq;

		Double_t Vx = GetPrimaryMCVertex_X(0);
		Double_t Vy = GetPrimaryMCVertex_Y(0);
		Double_t Bx = 0., By = 0., dist = 0.;
		m_vertex.x = Vx;
		m_vertex.y = Vy;

		if(bt == _SINGLE_HIT){

			Bx = (*blobsItr).GetBlobProperties().geoCenter_x*0.4;
			Bx -= ( (GetMatrixXdim()/2.) * 0.4 ) ;
			Bx *= -1.;

			By = (*blobsItr).GetBlobProperties().geoCenter_y*0.05;
			By -= ( (GetMatrixYdim()/2.) * 0.05 );
			By *= -1.;

			dist = MAFTools::CalcDistance(Vx,Vy,Bx,By);
			m_deviation_sh.push_back(dist);
			Log << MSG::INFO << "Distance single hit = " << dist << endreq;



		}else if(bt == _DOUBLE_HIT){

			//Bx = (*blobsItr).GetBlobProperties().weightedCenter_x*0.4;
			Bx = (*blobsItr).GetBlobProperties().geoCenter_x*0.4;
			Bx -= ( (GetMatrixXdim()/2.) * 0.4 ) ;
			Bx *= -1.;

			//By = (*blobsItr).GetBlobProperties().weightedCenter_y*0.05;
			By = (*blobsItr).GetBlobProperties().geoCenter_y*0.05;
			By -= ( (GetMatrixYdim()/2.) * 0.05 );
			By *= -1.;

			dist = MAFTools::CalcDistance(Vx,Vy,Bx,By);
			m_deviation_dh.push_back(dist);
			Log << MSG::INFO << "Distance double hit = " << dist << endreq;

		}

	}

	// Fill the output tree of this algorithm
	getMyTree()->Fill();

	// WARNING ! don't forget to clean up your variables for the next TTree::Fill call
	m_deviation_sh.clear();
	m_deviation_dh.clear();
	m_vertex.x = 0;
	m_vertex.y = 0;

}

void Bfieldtest::Finalize() {

	Log << MSG::INFO << "Finalize function !" << endreq;

}

#endif

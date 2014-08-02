
/**
 * Created automatically with MAFalda (Mi 30 Jul 2014 18:02:57 CEST)
 * MAFalda Author:  John Idarraga <idarraga@cern.ch>
 *
 * An example of how to use the clustering results
 * for further processing.
 */

#ifndef __TestClusterFilter_cpp
#define __TestClusterFilter_cpp

#include "TestClusterFilter.h"
#include "MAFTools.h"

using namespace MSG;

ClassImp(TestClusterFilter)

TestClusterFilter::TestClusterFilter() : MediPixAlgo(), CalibrationLoader(this) {

  // This value will be overridden by the configuration because it'll registered
  //  as a ConfigurationValue in the Init member of this class.
  m_minNPixels = 5;

}

void TestClusterFilter::Init(){

	Log << MSG::INFO << "Init function !" << endreq;


	// You will get an ntuple file containing a TTree with the name of this
	//  Algorithm.  The branches registered through getMyTree() get registered
	//  in that tree so you can fill them each time Execute() gets called.
	getMyTree()->Branch("clusterTOT", &m_clusterTOT);
	getMyTree()->Branch("clusterEnergy", &m_clusterEnergy);
	getMyTree()->Branch("hotestPixel", &m_hotestPixel);

	// A configuration value that can be tuned from the Viewer
	RegisterConfigurationValue(&m_minNPixels, "minNPixels");

}

void TestClusterFilter::Execute(){

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

	cluster cl;
	for( ; blobsItr != blobsVector.end() ; blobsItr++) {

		cl = *blobsItr;

		// Limit all this to clusters with a minimum size.
		// Note that m_minNPixels can be configured through the Viewer
		//  so you can reprocess and check results online.
		if(cl.bP.nPixels < m_minNPixels)
			continue;
		
		Log << MSG::INFO << "selected clusters" << endreq;



		// If the cluster passes the minimum requirements loop over the
		// the constituents of the cluster --> ClusterDescription
		list< pair < pair<int, int>, int > > cl_des = cl.GetClusterDescription();
		list< pair < pair<int, int>, int > >::iterator i = cl_des.begin();

		// Store the cluster TOT for output
		m_clusterTOT.push_back( cl.bP.clusterTOT );

		i = MAFTools::FindHotestPixel(&cl_des);

		m_hotestPixel.push_back( (*i).second);

		double calib_edep = 0.0, clusterEdep = 0.;
		int tot = 0;
		pair<int, int> pix;

		for ( ; i != cl_des.end() ; i++) {

			// pixel coordinates and tot
			pix = (*i).first;
			tot = (*i).second;

			// Use calibration to obtain E = Surrogate(TOT) for this pixel
			calib_edep = CalculateAndGetCalibEnergy(pix, tot);

			// Calculate the energy of the cluster
			clusterEdep += calib_edep;

		}

		// Store the cluster Energy calculated in the previous loop
		m_clusterEnergy.push_back( clusterEdep );

	}

	// Fill the output tree of this algorithm
	getMyTree()->Fill();

	// WARNING ! don't forget to clean up your variables for the next TTree::Fill call
	m_clusterEnergy.clear();
	m_clusterTOT.clear();
	m_hotestPixel.clear();

}

void TestClusterFilter::Finalize() {

	Log << MSG::INFO << "Finalize function !" << endreq;

}

#endif

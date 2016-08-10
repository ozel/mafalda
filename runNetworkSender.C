#include "AnalysisCore/MediPixAnalysisManager.h"
//#include "AnalysisCore/MediPixAnalysisCoreLinkDef.h"
#include "BlobsFinder/BlobsFinder.h"
#include "NetworkSender/NetworkSender.h"
/**
 * Created automatically with MAFalda (Di 26 Aug 2014 18:10:33 CEST)
 * MAFalda Author:  John Idarraga <idarraga@cern.ch>
 *
 * An example of a top layer file used to steer the job.
 * To run in a shell issue the command:
 *  $ root -l runNetworkSender.C
 */

TString def_hostname = "ozelipad";
TString host2 = "ozelmacpro"; //optional, can be empty
TString host3 = "";	      //optional, can be empty

void runNetworkSender(TString hostname = def_hostname){

	// Load the MediPix analysis lib
	gSystem->Load("libMediPixAnalysisCore.so");

	// Get an instance of the analysis manager and load data
	AnalysisManager mpxAnalysis("/tmp/frame_",kTRUE);

	// Blobs Finder.  This is a Clustering Algorithm
	BlobsFinder * bf = new BlobsFinder;
	mpxAnalysis.ConnectAlgo("BlobsFinder", bf);
	bf->changeOutputLevel(MSG::INFO);
	//bf->SetBorderExclusion(1);
	//bf->SetDiscontinuityTolerance(0);
	bf->ReadConfiguration();

	// Your algorithm --> NetworkSender
	NetworkSender * ac = new NetworkSender;
	ac->changeOutputLevel(MSG::INFO);
	mpxAnalysis.ConnectAlgo("NetworkSender", ac);
	ac->SetHostname(hostname, host2, host3);
	ac->SendRawTOTperPixel(kTRUE);
	// Setting up the calibration from this algorithm by loading
	//  the 4 calibration files and the clock used for calibration. 
	//  If you fail loading any of the 5 following pieces then the 
	//  calibration won't be available but you can still run the job.
	ac->SetCalibrationConfigFile_a("../calib/a.txt");
	ac->SetCalibrationConfigFile_b("../calib/b.txt");
	ac->SetCalibrationConfigFile_c("../calib/c.txt");
	ac->SetCalibrationConfigFile_t("../calib/t.txt");
	ac->SetCalibClk(10); // MHz
	ac->ReadConfiguration();
	ac->OutputLevel(MSG::INFO);

	// This is an special algorithm that works as a frames viewer
	//MPXViewer * v1 = new MPXViewer;
	//v1->changeOutputLevel(MSG::DEBUG);
	//mpxAnalysis.ConnectAlgo("MPXViewer", v1);
	//v1->SetCuts(10,10); // A minimum cut to skip uninteresting frames
	//v1->SetFrameTitle("Offline Analysis");

	// Get a list of the algorithms connected
	mpxAnalysis.DumpAlgoList();

	// If you want to setup an output filename yourself
	//  otherwise MAFalda builds something convenient
	//mpxAnalysis.SetOutputNtupleFilename("outputfile.root");

	// Run !
	mpxAnalysis.Run();         // all frames
	//mpxAnalysis.Run(0, 100); // range of frames
	//mpxAnalysis.Run(1);      // ony one frame

}


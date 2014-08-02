
/**
 * Created automatically with MAFalda (Mi 23 Jul 2014 16:04:23 CEST)
 * MAFalda Author:  John Idarraga <idarraga@cern.ch>
 *
 * An example of a top layer file used to steer the job.
 * To run in a shell issue the command:
 *  $ root -l runTest.C
 */

void runTest(){

	// Load the MediPix analysis lib
	gSystem->Load("libMediPixAnalysisCore.so");

	TGFileInfo fi;
	//fi.fFileTypes = ".root";
	new TGFileDialog(gClient->GetRoot(), gClient->GetRoot(), kFDOpen, &fi); 
	printf(fi.fFilename);
	//return;

	// Get an instance of the analysis manager and load data
	AnalysisManager mpxAnalysis(fi.fFilename);

	// Blobs Finder.  This is a Clustering Algorithm
	// BlobsFinder * bf = new BlobsFinder;
	// mpxAnalysis.ConnectAlgo("BlobsFinder", bf);
	// bf->changeOutputLevel(MSG::INFO);
	// bf->SetBorderExclusion(1);
	// bf->SetDiscontinuityTolerance(0);
	// bf->ReadConfiguration();

	/*
   	* Fast Clustering for single hits only
   	*/
  	SimpleClustering * sc = new SimpleClustering;
  	mpxAnalysis.ConnectAlgo("SimpleClustering", sc);
  	sc->changeOutputLevel(MSG::DEBUG);

	// Your algorithm --> Test
	Test * ac = new Test;
	mpxAnalysis.ConnectAlgo("Test", ac);
	// Setting up the calibration from this algorithm by loading
	//  the 4 calibration files and the clock used for calibration. 
	//  If you fail loading any of the 5 following pieces then the 
	//  calibration won't be available but you can still run the job.
	ac->SetCalibrationConfigFile_a("../calib/3kev/_Calib_a.txt");
	ac->SetCalibrationConfigFile_b("../calib/3kev/_Calib_b.txt");
	ac->SetCalibrationConfigFile_c("../calib/3kev/_Calib_c.txt");
	ac->SetCalibrationConfigFile_t("../calib/3kev/_Calib_t.txt");
	ac->SetCalibClk(10); // MHz
	ac->ReadConfiguration();
	ac->OutputLevel(MSG::INFO);

	// This is an special algorithm that works as a frames viewer
	MPXViewer * v1 = new MPXViewer;
	v1->changeOutputLevel(MSG::DEBUG);
	mpxAnalysis.ConnectAlgo("MPXViewer", v1);
	// v1->SetCuts(10,10); // A minimum cut to skip uninteresting frames
	// v1->SetFrameTitle("Offline Analysis");

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


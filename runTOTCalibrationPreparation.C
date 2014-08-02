
void runTOTCalibrationPreparation(TString fn){

  /**
   * Author: John Idarraga <idarraga@cern.ch> 
   * LAL, Orsay.
   *
   * C++ ROOT macro for analysis of MediPix data using multiformat
   * ROOT-based files and MAFalda - the MediPix Analysis Framework
   * 
   */

  /* Load the MediPix analysis lib */
  gSystem->Load("libMediPixAnalysisCore.so");
  
  /* Get an instance of the analysis manager */
  AnalysisManager mpxAnalysis( fn );

  mpxAnalysis.SetStoreGateOutputLevel(MSG::ERROR);

  //string outfile = "output.root";

  /* Blobs Finder */
  SimpleClustering * ac = new SimpleClustering;
  mpxAnalysis.ConnectAlgo("SimpleClustering", ac);
  //ac->changeOutputLevel(MSG::DEBUG);

  //BlobsFinder * bf = new BlobsFinder;
  //mpxAnalysis.ConnectAlgo("BlobsFinder", bf);
  //bf->changeOutputLevel(MSG::ERROR);
  //bf->SetBorderExclusion(0);
  //bf->SetDiscontinuityTolerance(0);
  //bf->ReadConfiguration();

  TOTCalibrationPreparation * cp = new TOTCalibrationPreparation;
  mpxAnalysis.ConnectAlgo("TOTCalibrationPreparation", cp);
  //cp->changeOutputLevel(MSG::DEBUG);

  /* This is an special algorithm that works as a
     frames viewer */
  MPXViewer * v1 = new MPXViewer;
  //v1->changeOutputLevel(MSG::DEBUG);
  //mpxAnalysis.ConnectAlgo("MPXViewer", v1);
  v1->SetCuts(0, 0);
  v1->SetFrameTitle("calib preparation");

  /* Get a list of the algorithms connected */
  mpxAnalysis.DumpAlgoList();

  /* If you want to setup an output filename yourself 
   * otherwise MAFalda builds something convenient
   */
  //mpxAnalysis.SetOutputNtupleFilename(outfile.c_str());
  
  /* run ! */
  mpxAnalysis.Run();         // all frames
  //mpxAnalysis.Run(0, 100); // range of frames
  //mpxAnalysis.Run(1);      // ony one frame
  
}

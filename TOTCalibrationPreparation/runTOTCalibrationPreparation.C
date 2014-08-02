
void runTOTCalibrationPreparation(){

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
  AnalysisManager mpxAnalysis("/home/idarraga/storage/B08_W0170_Erik/In_fluo/MPXNtuple_In_fluo_dataset02_10MHz.root");

  mpxAnalysis.SetStoreGateOutputLevel(MSG::ERROR);

  //string outfile = "output.root";

  /*
   * Fast Clustering for single hits only
   */
  SimpleClustering * ac = new SimpleClustering;
  mpxAnalysis.ConnectAlgo("SimpleClustering", ac);
  //ac->changeOutputLevel(MSG::DEBUG);

  /*
   * Calibration preparation dumps the Tree necessary to process the calibration with TOTCalib
   * see: http://svnweb.cern.ch/world/wsvn/idarraga
   */
  TOTCalibrationPreparation * cp = new TOTCalibrationPreparation;
  mpxAnalysis.ConnectAlgo("TOTCalibrationPreparation", cp);

  /* This is an special algorithm that works as a
     frames viewer */
  MPXViewer * v1 = new MPXViewer;
  //mpxAnalysis.ConnectAlgo("MPXViewer", v1);
  v1->SetCuts(10, 10);
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

 /**
 * MAFalda Top layer file in the form of a C++ ROOT macro
 *
 * Author: John Idarraga <idarraga@cern.ch>
 * University of Houston - 2012
 *
 * - Run this script in the following way
 *   $ root -l runExample.C
 * - For a full debug run look at share/runCintAttachGdbEmacs.C
 */

void runOctoCEA(){
  
  // Load the MediPix analysis lib
  gSystem->Load("libMediPixAnalysisCore.so");
  
  // Get an instance of the analysis manager, including input data
  // hint: it can also be a text file containing a list of input files
  AnalysisManager mpxAnalysis("/home/Andrii/DATA/Octopuce/DESY_test_beam/data/MPXNtuple_RUN005.root");

  /*
   * Clustering:
   *  BlobsFinder is a clustering algorithm.
   *  It supports discontinuities and other
   *  exceptions.  It is suitable for most
   *  applications.
   */
  BlobsFinder * bf = new BlobsFinder;
  mpxAnalysis.ConnectAlgo("BlobsFinder", bf);
  bf->changeOutputLevel(MSG::INFO);
  bf->SetBorderExclusion(0);
  bf->SetDiscontinuityTolerance(1);
  bf->ReadConfiguration("OctoCEA/configuration_algos_OctoCEA.txt");

  // Simple geo based Pattern Recognition
  PRBasicSpecies * pr1 = new PRBasicSpecies;
  mpxAnalysis.ConnectAlgo("PRBasicSpecies", pr1);
  pr1->ReadConfiguration("OctoCEA/configuration_algos_OctoCEA.txt");

  /*
   * AfterClusteringExample:
   *  This algorithms shows how to fetch the
   *  results of the clustering through the
   *  StoreGate and use it for further processing.
   */
  OctoCEA * ac = new OctoCEA;
  mpxAnalysis.ConnectAlgo("OctoCEA", ac);
  ac->changeOutputLevel(MSG::DEBUG);
  //ac->ReadConfiguration("OctoCEA/configuration_algos_OctoCEA.txt");

  // Alignment algorithm
  //OctoAlignment * al = new OctoAlignment;
  //mpxAnalysis.ConnectAlgo("OctoAlignment", al);

  // This is an special algorithm which works as a frames viewer
  MPXViewer * v1 = new MPXViewer;
  v1->changeOutputLevel(MSG::DEBUG);
  mpxAnalysis.ConnectAlgo("MPXViewer", v1);
  v1->SetCuts(0,0); 
  v1->SetFrameTitle("add a title");

  // Get a list of the algorithms connected
  mpxAnalysis.DumpAlgoList();

  // Use SetOutputNtuple() If you want to change the output filename.
  // Otherwise MAFalda will do it based on the input filename above
  //mpxAnalysis.SetOutputNtuple("myoutput.root");
  
  // run !
  //mpxAnalysis.Run();         // all frames
  mpxAnalysis.Run(0, 10); // range of frames
  //mpxAnalysis.Run(1);      // single frame

}

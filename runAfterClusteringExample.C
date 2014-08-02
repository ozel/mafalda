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

void runAfterClusteringExample(){
  
  // Load the MediPix analysis lib
  gSystem->Load("libMediPixAnalysisCore.so");
  
  // Get an instance of the analysis manager, including input data
  // hint: it can also be a text file containing a list of input files
  AnalysisManager mpxAnalysis("testdata/MPXNtuple_12C_TimePix.root");

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
  //bf->ReadConfiguration();

  PRBasicSpecies * pr = new PRBasicSpecies;
  //mpxAnalysis.ConnectAlgo("PRBasicSpecies", pr);
  pr->changeOutputLevel(MSG::INFO);

  /*
   * AfterClusteringExample:
   *  This algorithms shows how to fetch the
   *  results of the clustering through the
   *  StoreGate and use it for further processing.
   */
  AfterClusteringExample * ac = new AfterClusteringExample;
  //mpxAnalysis.ConnectAlgo("AfterClusteringExample", ac);
  ac->changeOutputLevel(MSG::DEBUG);
  // ac->ReadConfiguration();

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
  mpxAnalysis.Run();         // all frames
  //mpxAnalysis.Run(0, 100); // range of frames
  //mpxAnalysis.Run(1);      // single frame

}

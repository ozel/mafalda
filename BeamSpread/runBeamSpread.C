{

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
  //AnalysisManager mpxAnalysis("testdata/MPXNtuple_241Am_106Ru_137Cs_13keV_30deg_784_100V.root");
  AnalysisManager mpxAnalysis("/home/idarraga/geant4/allpix/MPXNtuple_AllPix_det_104.root");
  //AnalysisManager mpxAnalysis("/home/idarraga/geant4/allpix/positron4GeV_test3/MPXNtuple_AllPix_det_102.root");

  //string outfile = "output.root";

  /* Meta Data */
  MetaData * md = new MetaData;
  mpxAnalysis.ConnectAlgo("MetaData", md);

  /* Blobs Finder */
  BlobsFinder * bf = new BlobsFinder;
  mpxAnalysis.ConnectAlgo("BlobsFinder", bf);
  bf->changeOutputLevel(MSG::INFO);
  bf->SetBorderExclusion(1);
  bf->SetDiscontinuityTolerance(0);
  //bf->ReadConfiguration();

  /* Patter Recognition basic species */
  PRBasicSpecies * pr = new PRBasicSpecies;
  mpxAnalysis.ConnectAlgo("PRBasicSpecies", pr);
  pr->changeOutputLevel(MSG::INFO);
  pr->SetHeavyBlobOnOff(true);
  //pr->ReadConfiguration();

  // Beam Spread for EUDET telescope sim
  BeamSpread * bs = new BeamSpread;
  //bs->changeOutputLevel(MSG::DEBUG);
  mpxAnalysis.ConnectAlgo("BeamSpread", bs);
  bs->ReadConfiguration();

  /* This is an special algorithm that works as a
     frames viewer */
  MPXViewer * v1 = new MPXViewer;
  v1->changeOutputLevel(MSG::DEBUG);
  //mpxAnalysis.ConnectAlgo("MPXViewer", v1);
  //v1->SetCuts(10, 10);

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

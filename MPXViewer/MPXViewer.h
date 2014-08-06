/**
 * Author: John Idarraga <idarraga@cern.ch> , 2009
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef MPXViewer_h
#define MPXViewer_h

#include <TH2.h>
#include <TCanvas.h>
#include <TBrowser.h>
#include <iostream>
#include <TROOT.h>
#include <vector>
#include <TGLabel.h>
#include <TLatex.h>
#include <TPaletteAxis.h>

#include "MPXAlgo/MediPixAlgo.h"
#include "MPXAlgo/Highlighter.h"

#include "ViewerControl.h"
#include "Viewer_defs.h"


class MPXViewer : public MediPixAlgo {

public:

  MPXViewer();
  virtual ~MPXViewer(){};

  void Init();
  void Execute();
  void Finalize();

  /* overloaded from parent */
  AlgoSignalsHandler SignalToSteering();
  Bool_t SignalFlag();

  void SetCuts(Int_t nHitsInPadCut_i, Int_t nChargeInPadCut_i);
  void histoProperties(TH2I *, TCanvas *);
  void setHistoPalette(Int_t visMode);
  TString FindCurrentFile();
  Int_t FindCurrentFileId(TString);
  Int_t GetTotalNumberOfFiles(){return (int)(GetInputFilesMap().size());}; // FIXME REM
  Int_t GetNEntriesForFile(TString f){return GetInputFilesMap()[f];};		// FIXME

  Bool_t SearchHotPixel();
  void SetFrameTitle(TString ti){m_viewerTitle = ti;};


  TH2I * getFrameHist();

private:

  /* FIXME !!!!!!!!
     This thing is also defined in MPXAlgo ... we should be using
     this instance to pass to the Viewer control, not the instance in
     MPXAlgo !
  */
  TApplication * g_theApp;
  ViewerSteer * vSteer;
  TCanvas * c1;
  TBrowser * b1;
  MPXViewerControl * pMenu;


  Int_t nHitsInPadCut;
  Int_t nChargeInPadCut;

  Float_t  Saturation;
  Float_t  Maxlightness;
  Float_t  Minlightness;
  Float_t  MaxHue; // from 0 to 360
  Float_t  MinHue;
  Float_t  Lightness;

  Int_t     palette[MAX_COLORS];

  /* info in the Control panel */
  ControlUpdateInfo * updateInfo;
  ControlFeedbackInfo * feedBackInfo_Old;
  ControlFeedbackInfo * feedBackInfo_New;

  /* signals */
  Bool_t signalGenerated;

  /* A title */
  TString m_viewerTitle;

  ClassDef(MPXViewer, 1)
};

#endif

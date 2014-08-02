/**
 * Author: John Idarraga <idarraga@cern.ch> , 2008
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef MediPixAlgoTimer_h
#define MediPixAlgoTimer_h

#include <map>
#include <iostream>
#include <TROOT.h>
#include <TStopwatch.h>
#include <TString.h>

class MediPixAlgoTimer {

 public:
  MediPixAlgoTimer();
  virtual ~MediPixAlgoTimer() { };

  void SetupATimer(TString);
  void ContinueATimer(TString);
  void StopATimer(TString);
  Double_t GetRealTime(TString);
  Double_t GetCpuTime(TString);

  Double_t GetLastSlotRealTime(TString);
  Double_t GetLastSlotCPUTime(TString);

  void DumpTimers();

 private:

  Int_t nTimers;
  std::map<TString, TStopwatch> timers; // elapsed time
  std::map<TString, TStopwatch> timersInstant; // time between last and previous stop

};


#endif

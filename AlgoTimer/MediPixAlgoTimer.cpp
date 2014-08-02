/**
 * Author: John Idarraga <idarraga@cern.ch> , 2008
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef MediPixAlgoTimer_cxx
#define MediPixAlgoTimer_cxx

#include "MediPixAlgoTimer.h"

MediPixAlgoTimer::MediPixAlgoTimer(){

  nTimers = 0;

}

void MediPixAlgoTimer::SetupATimer(TString timerName){

  timers[timerName] = TStopwatch();
  timers[timerName].Reset();

  timersInstant[timerName] = TStopwatch();
  timersInstant[timerName].Reset();

}

void MediPixAlgoTimer::ContinueATimer(TString timerName){

  // start
  timers[timerName].Start(kFALSE); /* start but don't reset */
  timersInstant[timerName].Start(kTRUE); /* start resetting */

}
void MediPixAlgoTimer::StopATimer(TString timerName){
  timers[timerName].Stop();
  timersInstant[timerName].Stop();
}
Double_t MediPixAlgoTimer::GetRealTime(TString timerName){
  return timers[timerName].RealTime();
}
Double_t MediPixAlgoTimer::GetCpuTime(TString timerName){
  return timers[timerName].CpuTime();
}
Double_t MediPixAlgoTimer::GetLastSlotRealTime(TString timerName){
  return timersInstant[timerName].RealTime();
}
Double_t MediPixAlgoTimer::GetLastSlotCPUTime(TString timerName){
  return timersInstant[timerName].CpuTime();
}

void MediPixAlgoTimer::DumpTimers(){

  std::map<TString, TStopwatch>::iterator timerItr = timers.begin();


  std::cout << "Timers ------------------------------------" << std::endl;
  std::cout << "  AlgoName:     realtime, cputime " << std::endl;
  for( ; timerItr != timers.end() ; timerItr++)
    {
      std::cout << "  " << (*timerItr).first << ": ";
      std::cout << GetRealTime((*timerItr).first) << ", "; 
      std::cout << GetCpuTime((*timerItr).first) << " [s]" << std::endl;
    }
  std::cout << "-------------------------------------------" << std::endl;

}
#endif

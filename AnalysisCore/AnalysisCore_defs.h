/**
 * Author: John Idarraga <idarraga@cern.ch> , 2008
 * Medipix Group, Universite de Montreal
 *
 */

#ifndef AnalysisCore_defs_h
#define AnalysisCore_defs_h

namespace MPXDefs {

  enum Flags {
    SERIALIZE_ME = 0,
    DO_NOT_SERIALIZE_ME,
  };

  enum SpecialObjs {
    NIL = 0,
    CONF_INT,
    CONF_FLOAT,
    CONF_DOUBLE,
    CONF_BOOL,
    CONF,  // Configuration objects.  They won't be erased over the entire run
    // Starting from here objects to erase on an event per event basis.
    VIS,       // Visualization drawable objects.
    VIS_SKIP,  // Visualization skip frame objects.
    REGULAR, // all regular objects erasable on an event per event basis
  };

}

#define LEFT_MSG_TAB 30

namespace MSG  {
  enum Level {
    NIL = 0,
    LOOP_DEBUG,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    FATAL,
    ALWAYS,
    NUM_LEVELS
  };

  typedef struct {
    Int_t endLevel;
  } Endreq ;

}

#define MAX_OCCUPANCY_CONF_LINE 1024

#endif


/**
 * Created automatically with MAFalda (Di 26 Aug 2014 18:10:33 CEST)
 * MAFalda Author:  John Idarraga <idarraga@cern.ch>
 *
 * An example of how to use the clustering results
 * for further processing.
 */

#ifndef __NetworkSender_h
#define __NetworkSender_h

#include "MPXAlgo/MediPixAlgo.h"
#include "CalibrationLoader/CalibrationLoader.h"

#include "avro.h"


// This algorithm is using an object put in the StoreGate
//  by BlobsFinder. I need to know the object.
#include "BlobsFinder/BlobsFinder.h"

class NetworkSender : public MediPixAlgo , public CalibrationLoader {

public:

  NetworkSender();
  virtual ~NetworkSender() { };

  // You ought to implement Init(), Execute() and Finalize()
  //  when you inherit from MediPixAlgo.  This model gives you
  //  direct access to data and services.
  void Init();
  void Execute();
  void Finalize();
  void init_schema();
  void add_cluster(avro_writer_t db);
  void SetHostname(const char * hostname);

private:

  AllBlobsContainer * m_aB;
  Int_t m_minNPixels;
  const char * m_hostname;

  // for output
  vector<int> m_clusterTOT;
  vector<double> m_clusterEnergy;

  ClassDef(NetworkSender, 1)
};

#endif

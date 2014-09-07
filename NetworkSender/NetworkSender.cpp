
/**
 * Created automatically with MAFalda (Di 26 Aug 2014 18:10:33 CEST)
 * MAFalda Author:  John Idarraga <idarraga@cern.ch>
 *
 * An example of how to use the clustering results
 * for further processing.
 */

#ifndef __NetworkSender_cpp
#define __NetworkSender_cpp

#include "NetworkSender.h"
#include "MAFTools.h"
#include "TUDPSocket.h"



#include <avro.h>
#include <assert.h>

#define BUFF_LEN 510
avro_schema_t tpx_schema, cluster_schema, cluster_array_schema;
avro_datum_t tpx_frame, avro_cluster, avro_cluster_array;
avro_writer_t a_db;
char * buffer;
TUDPSocket * fSocket;

/* A simple schema for our tutorial */
const char  TPX_SCHEMA[] =
		"{\"name\":\"Clusters\",\
		  \"type\":{\
		  \"type\": \"array\",\
		  \"items\":{\
		     \"name\": \"Cluster\",\
		       \"type\": \"record\",\
		       \"fields\" : [\
		               {\"name\": \"energy\", \"type\": \"float\"},\
		                {\"name\": \"hits\", \"type\": \"integer\"}\
		              ]}}}";

/* Parse schema into a schema data structure */
void NetworkSender::init_schema(void)
{
		avro_schema_error_t schema_error;
		char jsontext[16 * 1024];
		FILE *fp;
		int rval;

		fp = fopen("tpx_schema.json", "r");
		rval = fread(jsontext, 1, sizeof(jsontext) - 1, fp);
		jsontext[rval] = '\0';

		if(avro_schema_from_json_length(jsontext, rval, &tpx_schema)){
		      fprintf(stderr, "Unable to parse tpxFrame schema");
		                exit(EXIT_FAILURE);
		}

//		fp = fopen("cluster_schema.json", "r");
//		rval = fread(jsontext, 1, sizeof(jsontext) - 1, fp);
//		jsontext[rval] = '\0';
//
//		if(avro_schema_from_json_length(jsontext, rval, &cluster_schema)){
//		      fprintf(stderr, "Unable to parse Cluster schema");
//		                exit(EXIT_FAILURE);
//		}

		tpx_frame = avro_record(tpx_schema);


		cluster_array_schema = avro_schema_get_subschema(tpx_schema, "clusterArray");

		cluster_schema = avro_schema_record("Cluster", NULL);
		avro_schema_record_field_append(cluster_schema, "id", avro_schema_int());
		avro_schema_record_field_append(cluster_schema, "energy", avro_schema_float());
		avro_schema_record_field_append(cluster_schema, "xi", avro_schema_bytes());
		avro_schema_record_field_append(cluster_schema, "yi", avro_schema_bytes());
		avro_schema_record_field_append(cluster_schema, "ei", avro_schema_bytes());

		tpx_frame = avro_record(tpx_schema);

		//avro_schema_t cluster_schema = avro_schema_get_subschema(cluster_array_schema, "Cluster"); //doesn't work, why?



		fprintf(stderr,"%d %d %d \n", tpx_frame, cluster_array_schema, cluster_schema);

//        if (avro_schema_from_json_literal(TPX_SCHEMA, &tpx_schema)) {
//                fprintf(stderr, "Unable to parse TPX-SCHEMA schema\n");
//                exit(EXIT_FAILURE);
//        }
}

void NetworkSender::add_cluster(avro_writer_t db)
{
//        avro_datum_t tpx_frame = NULL;



//        avro_datum_t energy_datum = avro_float(0.42);
//        avro_datum_t id_datum = avro_int32(42);
//        avro_datum_t bytes_datum = avro_bytes(new char[42, 43, 44, 45, 46],5);
//
//        if ( avro_record_set(avro_cluster, "id", id_datum))                 {
//                                 fprintf(stderr, "Unable to create id datum structure\n");
////                                  exit(EXIT_FAILURE);
//                 }
//
//        if ( avro_record_set(avro_cluster, "energy", energy_datum))                 {
//                         fprintf(stderr, "Unable to create energy datum structure\n");
////                          exit(EXIT_FAILURE);
//         }
//
//        avro_record_set(avro_cluster, "width", id_datum);
//        avro_record_set(avro_cluster, "height", id_datum);
//        avro_record_set(avro_cluster, "hits", bytes_datum);

		fprintf(stderr, "avro cluster size %i", avro_size_data(a_db, tpx_schema,avro_cluster));

		//if(avro_writer_tell(a_db) +  < BUF_LEN ){
			if ( avro_array_append_datum(avro_cluster_array, avro_cluster) ) {
                	fprintf(stderr, "Unable to create cluster_array datum structure\n");
                	//                exit(EXIT_FAILURE);
				}
		//}



//        printf("data len = %ld\n", avro_writer_tell(db));

        /* Decrement all our references to prevent memory from leaking */
//        avro_datum_decref(tpx_frame);
//        avro_datum_decref(avro_cluster);
//        avro_datum_decref(energy_datum);

        //fprintf(stdout, "Successfully added %s, %s id=%"PRId64"\n", last, first, id);
}

using namespace MSG;


ClassImp(NetworkSender)

NetworkSender::NetworkSender() : MediPixAlgo(), CalibrationLoader(this) {

  // This value will be overridden by the configuration because it'll registered
  //  as a ConfigurationValue in the Init member of this class.
  m_minNPixels = 1;

}

void NetworkSender::SetHostname(const char * hostname){
	m_hostname = hostname;
}


void NetworkSender::Init(){

	Log << MSG::INFO << "Init function !" << endreq;


	// You will get an ntuple file containing a TTree with the name of this
	//  Algorithm.  The branches registered through getMyTree() get registered
	//  in that tree so you can fill them each time Execute() gets called.
	getMyTree()->Branch("clusterTOT", &m_clusterTOT);
	getMyTree()->Branch("clusterEnergy", &m_clusterEnergy);

	// A configuration value that can be tuned from the Viewer
	RegisterConfigurationValue(&m_minNPixels, "minNPixels");

	buffer = new char[BUFF_LEN];
	a_db = avro_writer_memory(buffer, BUFF_LEN);
	init_schema();
	Log << MSG::INFO <<  "Creating TUDPSocket" << endreq;
	fSocket = new TUDPSocket(m_hostname, 8123);

	if (!fSocket || !fSocket->IsValid()) {
		Log << MSG::ERROR << "cannot connect to host:" << m_hostname << endreq;

	}

	   fSocket->SetOption(kNoBlock, 1);

}

void NetworkSender::Execute(){

	// Ask the store gate if the previous algorithm (BlobsFinder --> reponsible for clustering)
	//  sent any objects to the StoreGate.
	Int_t lastObject = GetNumberOfObjectsWithAuthor("BlobsFinder");
	if(lastObject == 0)
		return;

	// If so, get the pointer to the last object.  BlobsFinder, as it comes out of the box, sends
	//  a single object containing all clusters.
	m_aB = (AllBlobsContainer *) GetObjectFromAuthor("BlobsFinder", lastObject-1);

	// AllBlobsContainer is a box full of Blobs(Clusters). Now we can iterate over all clusters inside.
	vector<blob> blobsVector = m_aB->GetBlobsVector();
	Log << MSG::INFO << "Number of blobs from clustering = " << (Int_t) blobsVector.size() << endreq;
	vector<blob>::iterator blobsItr = blobsVector.begin(); //allBlobs.begin();


	avro_cluster_array = avro_array(cluster_array_schema);



	cluster cl;
	int id = 0;
	for( ; blobsItr != blobsVector.end() ; blobsItr++) {

		cl = *blobsItr;

		// Limit all this to clusters with a minimum size.
		// Note that m_minNPixels can be configured through the Viewer
		//  so you can reprocess and check results online.
		if(cl.bP.nPixels < m_minNPixels)
			continue;
		
		// If the cluster passes the minimum requirements loop over the
		// the constituents of the cluster --> ClusterDescription
		list< pair < pair<int, int>, int > > cl_des = cl.GetClusterDescription();
		list< pair < pair<int, int>, int > >::iterator i = cl_des.begin();

		// Store the cluster TOT for output
		m_clusterTOT.push_back( cl.bP.clusterTOT );

		avro_cluster = avro_record(cluster_schema);


		avro_record_set(avro_cluster, "id", avro_int32(id));

		double calib_edep = 0.0, clusterEdep = 0.;
		int tot = 0;
		pair<int, int> pix;

		char * xi = new char[cl.bP.nPixels]();
		char * yi = new char[cl.bP.nPixels]();
		char * ei = new char[cl.bP.nPixels]();
		int count =0;
		float totalTOT = 0.0;
		for ( ; i != cl_des.end() ; i++) {

			// pixel coordinates and tot
			pix = (*i).first;
			tot = (*i).second;
			xi[count]=pix.first;
			yi[count]=pix.second;
			ei[count]=(*i).second;

			// Use calibration to obtain E = Surrogate(TOT) for this pixel
			calib_edep = CalculateAndGetCalibEnergy(pix, tot);
			totalTOT += (float)tot;

			// Calculate the energy of the cluster
			clusterEdep += calib_edep;
//			if (count==0){
				Log << MSG::DEBUG << "pixel_x:" << pix.first << " pixel_y: " << pix.second << " tot:" << tot <<  endreq;
//			}
			count++;

		}
		Log << MSG::INFO << "clusterSize " << cl.bP.nPixels << endreq;
		Log << MSG::INFO << "totalTOT " << totalTOT << endreq;

		avro_record_set(avro_cluster, "xi", avro_bytes(xi, cl.bP.nPixels));
		avro_record_set(avro_cluster, "yi", avro_bytes(yi, cl.bP.nPixels));
		avro_record_set(avro_cluster, "ei", avro_bytes(ei, cl.bP.nPixels));

		// Store the cluster Energy calculated in the previous loop
		m_clusterEnergy.push_back( clusterEdep );
		avro_record_set(avro_cluster, "energy", avro_float(totalTOT));


		add_cluster(a_db);

		id++;

	}

	// Fill the output tree of this algorithm
	getMyTree()->Fill();

    if ( avro_record_set(tpx_frame, "clusterArray", avro_cluster_array))                 {
                         fprintf(stderr, "Unable to create Clusters datum structure\n");
                          exit(EXIT_FAILURE);
         }


      if (avro_write_data(a_db, tpx_schema, tpx_frame)) {
              fprintf(stderr,
                      "Unable to write tpx_frame %d of %d bytes to memory buffer\nMessage: %s\n", avro_writer_tell(a_db), avro_strerror());

//              	  avro_array_(avro_cluster_array, avro_cluster);

//              avro_writer_reset(a_db);
//              exit(EXIT_FAILURE);
      }

    int db_size = avro_writer_tell(a_db);
    printf("final data len = %ld\n", db_size);

    TString msg = "80	80	80\n81	80	80\n82	80	80\n83	80	80\n84	80	80\n85	80	80\n";

    Log << MSG::INFO  << "new frame:" << a_db << endreq;

    if (fSocket->SendRaw(buffer, db_size) == -1) {
    	Log << MSG::ERROR << "error sending command to host" << endreq;
    }

    avro_writer_reset(a_db);
//    avro_datum_decref(avro_cluster_array);
//   avro_datum_decref(avro_cluster);
//    avro_datum_decref(tpx_frame);

	// WARNING ! don't forget to clean up your variables for the next TTree::Fill call
	m_clusterEnergy.clear();
	m_clusterTOT.clear();

}

void NetworkSender::Finalize() {

	Log << MSG::INFO << "Finalize function !" << endreq;
    avro_datum_decref(avro_cluster);
    avro_datum_decref(avro_cluster_array);
    avro_datum_decref(tpx_frame);
    avro_datum_decref(cluster_schema);
    avro_datum_decref(cluster_array_schema);
    avro_datum_decref(tpx_schema);
}

#endif

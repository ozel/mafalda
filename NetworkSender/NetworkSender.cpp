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


#include <string.h> 
#include <avro.h>
#include <TString.h>
//#include <avro/codec.h>
#include <assert.h>

#define BUFF_LEN 1472 //508
avro_schema_t tpx_schema, cluster_schema, cluster_array_schema, energy_array_schema, x_array_schema, y_array_schema;
avro_datum_t tpx_frame, avro_cluster, avro_cluster_array, avro_energy_array, avro_x_array, avro_y_array;
avro_writer_t a_db;
char * buffer;
TUDPSocket * fSocket;
TUDPSocket * fSocket2;
TUDPSocket * fSocket3;

unsigned char * xi;
unsigned char * yi;
unsigned char * ei;


//Do not call me.
long getUTF8size(const wchar_t *string){
    if (!string)
        return 0;
    long res=0;
    for (;*string;string++){
        if (*string<0x80)
            res++;
        else if (*string<0x800)
            res+=2;
        else
            res+=3;
    }
    return res;
}

/*
 string: a wchar_t C string (nul terminated)
 Return value: a UTF-8-encoded C string.

 The function handles memory allocation on its own.

 Limitations: Only handles the range [U+0000;U+FFFF], higher code points are
 changed to '?'.

 Assumptions: sizeof(wchar_t)>=2
 */
char *WChar_to_UTF8(const wchar_t *string){
    long fSize=getUTF8size(string);
    char *res=new char[fSize+1];
    res[fSize]=0;
    if (!string)
        return res;
    long b=0;
    for (;*string;string++,b++){
        if (*string<0x80)
            res[b]=(char)*string;
        else if (*string<0x800){
            res[b++]=(*string>>6)|192;
            res[b]=*string&63|128;
        }else{
            res[b++]=(*string>>12)|224;
            res[b++]=((*string&4095)>>6)|128;
            res[b]=*string&63|128;
        }
    }
    return res;
}

/* Parse schema into a schema data structure */
void NetworkSender::init_schema(void)
{
		//avro_schema_error_t schema_error;
		char jsontext[32 * 1024];
		FILE *fp;
		int rval;
		
		if(m_send_raw_tot){
			fp = fopen("tpx_schema.json", "r");
		} else {
			fp = fopen("tpx_schema_tot_scaled.json", "r");			
		}
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
		energy_array_schema = avro_schema_array(avro_schema_int());
		x_array_schema = avro_schema_array(avro_schema_int());
		y_array_schema = avro_schema_array(avro_schema_int());
		//energy_array_schema = avro_schema_get_subschema(cluster_array_schema, "ei");
		if (!energy_array_schema)
			Log << MSG::ERROR << "could not find ei array in schema" << endreq;
		


		//avro_schema_t cluster_schema = avro_schema_get_subschema(cluster_array_schema, "Cluster"); //doesn't work, why?



		fprintf(stderr,"%ld %ld %ld %ld\n",tpx_frame, cluster_array_schema, cluster_schema, energy_array_schema);

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

//		fprintf(stderr, "avro cluster size %i\n", avro_size_data(a_db, tpx_schema,avro_cluster));

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
  m_send_raw_tot = kTRUE;
}

void NetworkSender::SetHostname(TString hostname, TString hostname2, TString hostname3 ){
	m_hostname = hostname;
	m_hostname2 = hostname2;
	m_hostname3 = hostname3;
}

void NetworkSender::SendRawTOTperPixel(bool send_raw_tot ){
        // if set kTRUE in run macro,full range of TOT is send _per_pixel_
	// if set kFALSE in run macro, TOT is scaled down to 8 bit to fit in one avro_byte
	// the total_cluster energy is independent and always send as calibarted energy value in keV!
	m_send_raw_tot = send_raw_tot;
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
	if(!m_hostname2.IsNull()) {
		Log << MSG::INFO <<  "setting up hostname2" << endreq; 		
		fSocket2 = new TUDPSocket(m_hostname2, 8123);
	        fSocket2->SetOption(kKeepAlive, 1);
        	//fSocket2->SetOption(kNoDelay, 1);
	        fSocket2->SetOption(kNoBlock, 1);
       	} if(!m_hostname3.IsNull()) {
		Log << MSG::INFO <<  "setting up hostname3" << endreq;
		fSocket3 = new TUDPSocket(m_hostname3, 8123);	
		fSocket3->SetOption(kNoBlock, 1);
       		fSocket3->SetOption(kKeepAlive, 1);
	} if (!fSocket || !fSocket->IsValid()) {
		Log << MSG::ERROR << "cannot connect to host:" << m_hostname << endreq;
	}

	fSocket->SetOption(kKeepAlive, 1);
	//fSocket->SetOption(kNoDelay, 1);
	fSocket->SetOption(kNoBlock, 1);

	xi = new unsigned char[256*256]();
	yi = new unsigned char[256*256]();
	ei = new unsigned char[256*256]();
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
	Log << MSG::DEBUG << "Number of blobs from clustering = " << (Int_t) blobsVector.size() << endreq;
	vector<blob>::iterator blobsItr = blobsVector.begin(); //allBlobs.begin();

	cluster_schema = avro_schema_record("Cluster", NULL);
	avro_schema_record_field_append(cluster_schema, "id", avro_schema_int());
	avro_schema_record_field_append(cluster_schema, "energy", avro_schema_float());
	avro_schema_record_field_append(cluster_schema, "center_x", avro_schema_float());
	avro_schema_record_field_append(cluster_schema, "center_y", avro_schema_float());
	//avro_schema_record_field_append(cluster_schema, "xi", avro_schema_bytes());
	//avro_schema_record_field_append(cluster_schema, "yi", avro_schema_bytes());
	avro_schema_record_field_append(cluster_schema, "xi", x_array_schema);
	avro_schema_record_field_append(cluster_schema, "yi", y_array_schema);
	if(m_send_raw_tot) {	
		if(avro_schema_record_field_append(cluster_schema, "ei", energy_array_schema))//avro_schema_array(avro_schema_int()));
		Log << MSG::ERROR << "could not append ei record field to schema" << avro_strerror() << endreq;	
	} else {
		avro_schema_record_field_append(cluster_schema, "ei", avro_schema_bytes());
	}
	tpx_frame = avro_record(tpx_schema);

	avro_cluster_array = avro_array(cluster_array_schema);
	//avro_energy_array = avro_array(energy_array_schema);


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

                double calib_edep = 0.0, clusterEdep = 0.;
                int tot = 0;
                pair<int, int> pix;

                calib_edep = CalculateAndGetCalibEnergy((*i).first, (*i).second);
                if(calib_edep < 4.0 && cl.bP.nPixels == 1){
                  continue; //skip noisy pixels
		}


		// Store the cluster TOT for output
		//m_clusterTOT.push_back( cl.bP.clusterTOT );
		
		avro_x_array = avro_array(x_array_schema);
		avro_y_array = avro_array(y_array_schema);

		avro_energy_array = avro_array(energy_array_schema);

		avro_cluster = avro_record(cluster_schema);


		avro_record_set(avro_cluster, "id", avro_int32(id));



//		xi[cl.bP.nPixels] = '\0';
		int xcount =0;
		int ycount =0;
		int ecount =0;
		for ( ; i != cl_des.end() ; i++) {

			// pixel coordinates and tot
			pix = (*i).first;
			tot = (*i).second;

			// we need to escape the zero value because Avro bytes are parsed as UTF8 string in the receiver
			// we use modified UTF8 here: 0 -> 0xC0, 0x80
//			if(pix.first == 0){
//				xi[xcount++] = 0xC0;
//				xi[xcount]   = 0x80;
//
//			} else {
//				xi[xcount]=pix.first;
//			}
//			if(pix.second == 0){
//				yi[ycount++] = 0xC0;
//				yi[ycount]   = 0x80;
//			} else{
//				yi[ycount]=pix.second;
//			}

			// Use calibration to obtain E = Surrogate(TOT) for this pixel
			calib_edep = CalculateAndGetCalibEnergy(pix, tot);
			
			if((pix.first >= 0) && (pix.second >= 0)){
				xi[xcount] = pix.first;
				yi[ycount] = pix.second;
				avro_datum_t i32_datum_x = avro_int32((int)pix.first);
				avro_datum_t i32_datum_y = avro_int32((int)pix.second);
				avro_array_append_datum(avro_x_array, i32_datum_x);
				avro_array_append_datum(avro_y_array, i32_datum_y);

				if(m_send_raw_tot) {
					avro_datum_t i32_datum = avro_int32(tot);
			                int rval = avro_array_append_datum(avro_energy_array, i32_datum);
			                //avro_datum_decref(i32_datum);
		                	if (rval) {
						Log << MSG::ERROR << "could not append energy datum" << endreq;
                		        	//exit(EXIT_FAILURE);
                			}
				} else {
					//tot should be never zero
					//we scale it here from 10bit (Timepix1) down to 8bit
			                // no! 11810 bit is maxium count
					if (tot > 1500) tot = 1500; //limit at ~1.5 MeV
						ei[ecount] = round((tot*256.0)/1500);
					//(x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
					//ei[ecount] = floor( (calib_edep - 4 ) * (256.0) / 1000.0);
					if (!ei[ecount])
						ei[ecount] = 1;  //~4kev if calibrated
				}
			}



			// Calculate the energy of the cluster
			clusterEdep += calib_edep;
//			if (count==0){
				Log << MSG::DEBUG << "x:" << pix.first << " y: " << pix.second << " tot:" << tot <<  " e:" << calib_edep << "keV" << endreq;
//				Log << MSG::DEBUG << "pixel_x:" << xi[xcount] << " pixel_y: " << yi[ycount] << " tot:" << ei[ecount] <<  endreq;
//			}
			xcount++;
			ycount++;
			ecount++;

		}
		Log << MSG::INFO << "clusterSize " << cl.bP.nPixels << endreq;
		Log << MSG::INFO << "clusterTOT " << cl.bP.clusterTOT  << endreq;
		Log << MSG::INFO << "cluster energy " << clusterEdep  << endreq;




		//avro_record_set(avro_cluster, "xi", avro_bytes((char *)xi, xcount));
		//avro_record_set(avro_cluster, "yi", avro_bytes((char *)yi, ycount));
		avro_record_set(avro_cluster, "xi", avro_x_array);
		avro_record_set(avro_cluster, "yi", avro_y_array);
		if(m_send_raw_tot){
			if(avro_record_set(avro_cluster, "ei", avro_energy_array))
				Log << MSG::ERROR << "could not set ei record field" << avro_strerror() << endreq;
		} else {
			avro_record_set(avro_cluster, "ei", avro_bytes((char *)ei, ecount));
		}
		// Store the cluster Energy calculated in the previous loop
		//m_clusterEnergy.push_back( clusterEdep );
		avro_record_set(avro_cluster, "energy", avro_float((float)clusterEdep));//cl.bP.clusterTOT));

		avro_record_set(avro_cluster, "center_x", avro_float(cl.bP.geoCenter_x));
		avro_record_set(avro_cluster, "center_y", avro_float(cl.bP.geoCenter_y));

		add_cluster(a_db);

	    	for(int i=0; i < 256; i++) {
			  xi[i]=0;
			  yi[i]=0;
			  ei[i]=0;
	    	}

		if(avro_writer_tell(a_db) > (BUFF_LEN-10)) break;
		id++;

	}

	// Fill the output tree of this algorithm
	//getMyTree()->Fill();

    if ( avro_record_set(tpx_frame, "clusterArray", avro_cluster_array))                 {
                         fprintf(stderr, "Unable to create Clusters datum structure\n");
                          exit(EXIT_FAILURE);
         }


//    avro_codec_t codec = (avro_codec_t) avro_new(struct avro_codec_t_);
//
//    codec->block_size=BUFF_LEN;
//
//
//    if (!codec) {
//    	fprintf(stderr,"Unable to allocate new codec\n");
//    	}
//    int rval = avro_codec(codec, "deflate");
//    if (rval) {
//    	fprintf(stderr,"Unable to write register deflate codec\nMessage: %s\n", avro_strerror());
//    	avro_codec_reset(codec);
//
//    }
//    if(avro_codec_encode(codec,buffer, BUFF_LEN)){
//    	 fprintf(stderr,
//    	                      "Unable to encode with codec\nMessage: %s\n", avro_strerror());
//
//    }
//
//    Log << MSG::DEBUG << "byte after deflate:" << codec->used_size << endreq;

      if (avro_write_data(a_db, tpx_schema, tpx_frame)) {
              fprintf(stderr,"Unable to write tpx_frame %i of %ld bytes to memory buffer\nMessage: %s\n", id, avro_writer_tell(a_db), avro_strerror());

//              	  avro_array_(avro_cluster_array, avro_cluster);

              	//avro_writer_reset(a_db);
		//avro_datum_decref(avro_cluster_array);
   		//avro_datum_decref(avro_cluster);
		//m_clusterEnergy.clear();
		//m_clusterTOT.clear();
		// exit(EXIT_FAILURE);
      }

    int db_size = avro_writer_tell(a_db);
    printf("final data len = %d\n", db_size);

    TString msg = "80	80	80\n81	80	80\n82	80	80\n83	80	80\n84	80	80\n85	80	80\n";

//    Log << MSG::DEBUG  << "new frame:" << a_db << endreq;

    
    if (!db_size || fSocket->SendRaw(buffer, db_size, kDontBlock) == -1) {
    	Log << MSG::ERROR << "error sending command to host" << endreq;
    }
    if(fSocket2)
    	fSocket2->SendRaw(buffer, db_size, kDontBlock);
    if(fSocket3)
	fSocket3->SendRaw(buffer, db_size, kDontBlock);

    avro_writer_reset(a_db);
    //avro_codec_reset(codec);
   avro_datum_decref(avro_x_array);
   avro_datum_decref(avro_y_array);
   avro_datum_decref(avro_energy_array);   
   avro_datum_decref(avro_cluster_array);
   avro_datum_decref(avro_cluster);
//    avro_datum_decref(tpx_frame);

	// WARNING ! don't forget to clean up your variables for the next TTree::Fill call


   m_clusterEnergy.clear();
   m_clusterTOT.clear();

}

void NetworkSender::Finalize() {

	Log << MSG::INFO << "Finalize function !" << endreq;
    avro_datum_decref(avro_cluster);
    avro_datum_decref(avro_x_array);
    avro_datum_decref(avro_y_array);
    avro_datum_decref(avro_energy_array);
    avro_datum_decref(avro_cluster_array);
    avro_datum_decref(tpx_frame);
 
   
    avro_schema_decref(cluster_schema);
    avro_schema_decref(cluster_array_schema);
    avro_schema_decref(x_array_schema);
    avro_schema_decref(y_array_schema);
    avro_schema_decref(energy_array_schema);	    
    avro_schema_decref(tpx_schema);
}

#endif

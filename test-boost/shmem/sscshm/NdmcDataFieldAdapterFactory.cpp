/*
 * NdmcDataFieldAdapterFactory.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: ran
 */



#include <boost/format.hpp>
#include <rashm/FieldAdapterFactory.h>
#include "NdmcData.h"
#include "ssc_signal_values.h"


namespace rashm {

template<>
void FieldAdapterFactory<ran::ssc::NdmcData, ran::ssc::signal_values>::init() {

    registerFactoryFun("depth", "Depth", "Wert von Feld depth (double)", []( VData const& d, value_frame_t& f ) {
    	if ( d.data.depth ) {
    		f.value = *d.data.depth;
    	} else {
    		f.valid = false;
    	}
    });

}

}


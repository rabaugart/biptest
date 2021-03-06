/*
 * DepthDataFieldAdapterFactory.cpp
 *
 *  Created on: Jan 31, 2018
 *      Author: ran
 */



#include <boost/format.hpp>
#include <rashm/FieldAdapterFactory.h>
#include "DepthData.h"
#include "ssc_signal_values.h"


namespace rashm {

template<>
void FieldAdapterFactory<ran::ssc::DepthData, ran::ssc::signal_values>::init() {

    registerFactoryFun("actual", "Actual depth [m]", "Actual depth (double)", []( VData const& d, value_frame_t& f ) {
    	if ( d.data.actualDepth ) {
    		f.value = *d.data.actualDepth;
    	} else {
    		f.valid = false;
    	}
    });

    registerFactoryFun("ordered", "Ordered depth [m]", "Ordered depth (double)", []( VData const& d, value_frame_t& f ) {
        if ( d.data.orderedDepth ) {
            f.value = *d.data.orderedDepth;
        } else {
            f.valid = false;
        }
    });

}

}



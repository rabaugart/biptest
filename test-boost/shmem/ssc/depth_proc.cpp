/*
 * depth_proc.cpp
 *
 *  Created on: 29.01.2018
 *      Author: netz
 */

#include <boost/chrono.hpp>

#include "NdmcData.h"
#include "DepthData.h"
#include "rashm/SegmentReader.h"
#include "rashm/SegmentWriter.h"

using ran::ssc::NdmcData;
using ran::ssc::DepthData;

int main(int argc, char** argv) {

    rashm::SegmentReader<NdmcData> reader;

    const boost::posix_time::microseconds timeout { 2000000 };

    rashm::SegmentWriter<DepthData> writer;
    DepthData out;

    while (true) {
        try {
            NdmcData in = reader.timed_wait_for(timeout);

            if ( in.depth && *in.depth > 0 ) {
                out.actualDepth = in.depth;
            } else {
                out.actualDepth = boost::none;
            }

            writer = out;
            std::cout << "Read: " << in << " Written " << out << std::endl;

        } catch (rashm::timeout_error const &) {
            std::cout << "Timeout, written " << out << std::endl;
            out.actualDepth = boost::none;
            writer = out;
        }
    }
}


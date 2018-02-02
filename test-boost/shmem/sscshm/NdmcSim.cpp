/*
 * NdmcSim.cpp
 *
 *  Created on: 29.01.2018
 *      Author: netz
 */

#include <thread>
//#include <math>

#include "rashm/SegmentWriter.h"
#include "NdmcData.h"

using ran::ssc::NdmcData;

int main(int argc, char** argv) {

    rashm::SegmentWriter<NdmcData> writer;

    NdmcData data;
    size_t i=0;
    while (true) {

        data.depth = std::sin(i/20.0*2.0*3.194)*20+18;

        writer = data;
        i = (i+1) % 20;
        std::cout << "Written " << data << std::endl;

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    return 0;
}


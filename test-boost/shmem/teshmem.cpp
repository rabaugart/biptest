/*
 * teshmem.cpp
 *
 *  Created on: 01.12.2017
 *      Author: netz
 */

#include "rashm/SegmentWriter.h"
#include "rashm/SegmentReader.h"
#include "test/TestDataA.h"

using namespace rashm;

int main(int argc, char **argv) {
    {
        typedef SegmentWriter<TestDataA> seg_t;
        seg_t seg;
        std::cout << "Name: " << seg.segmentName() << std::endl;
    }

    {
        typedef SegmentReader<TestDataA> reader_t;
        reader_t r;
        TestDataA const td = r.get();

        if (!r.writerIsPresent()) {
            std::cout << "No writer present" << std::endl;
        }
    }

    std::cout << "teshmem ok" << std::endl;

    //seg.removeSegment();
    return 0;
}


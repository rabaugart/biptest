/*
 * teshmem.cpp
 *
 *  Created on: 01.12.2017
 *      Author: netz
 */

#include <thread>
#include "rashm/SegmentWriter.h"
#include "rashm/SegmentReader.h"
#include "test/TestDataA.h"

using namespace rashm;

int main(int argc, char **argv) {
    typedef SegmentWriter<TestDataA> writer_t;
    typedef SegmentReader<TestDataA> reader_t;

    writer_t::removeSegment();

    {
        writer_t seg;
        TestDataA td;
        seg = td;
        std::cout << "Name: " << seg.segmentName() << " c:" << seg.counter() << std::endl;
    }

    {
        reader_t r;
        TestDataA const td = r.get();

        if (!r.writerIsPresent()) {
            std::cout << "No writer present" << std::endl;
        }

        reader_t r2;
        TestDataA const td2 = r2.get();
    }

    {
        TestDataA tdr;
        std::thread t1([&tdr]() {
            reader_t r;
            tdr = r.wait();
        });
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "Writer starting" << std::endl;
        writer_t seg;
        TestDataA td { 123.4, 77 };
        seg = td;
        std::cout << "Set" << std::endl;
        t1.join();
        std::cout << "Joined: " << td.a << " "
                << (td == tdr ? "equal" : "unequal") << " counter:"
                << seg.counter() << std::endl;
    }

    std::cout << "teshmem ok" << std::endl;

    writer_t::removeSegment();
    return 0;
}


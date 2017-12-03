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
    typedef SegmentWriter<TestDataA,TIdA> writer_t;
    typedef SegmentReader<TestDataA,TIdA> reader_t;

    writer_t::removeSegment();

    {
        writer_t seg;
        TestDataA td;
        seg = td;
        std::cout << "Name: " << seg.segmentName() << " c:" << seg.counter()
                << std::endl;
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
        TestDataA tdr1, tdr2;
        std::thread t1([&tdr1,&tdr2]() {
            reader_t r;
            tdr1 = r.wait();
            tdr2 = r.wait();
        });

        reader_t r2;

        // If the writer starts to fast the reader will miss the write operation and wait indefinitely
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::cout << "Writer starting" << std::endl;
        writer_t seg;
        TestDataA td1 { 123.4, 77 };
        seg = td1;
        std::cout << "Set" << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        seg = TestDataA { 456.7, 99 };
        t1.join();
        std::cout << "Joined: " << td1.a << " "
                << (td1 == tdr1 ? "equal" : "unequal") << " "
                << (tdr2 == r2.get() ? "equal" : "unequal") << " c:"
                << seg.counter() << std::endl;
    }

    std::cout << "teshmem ok" << std::endl;

    writer_t::removeSegment();
    return 0;
}


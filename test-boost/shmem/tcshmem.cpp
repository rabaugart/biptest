/*
 * tcshmem.cpp
 *
 *  Created on: 15.11.2017
 *      Author: netz
 */

//#define BOOST_INTERPROCESS_POSIX_PROCESS_SHARED

#include <algorithm>
#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/interprocess_sharable_mutex.hpp>
#include <boost/interprocess/sync/interprocess_condition_any.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <boost/format.hpp>
#include "test/MsgCollector.h"
#include "test/ComProc.h"
#include "test/SubProc.h"

namespace bip = boost::interprocess;

typedef bip::string ipstring;
static const ipstring SH_NAME_C { "myshared" };
#define SH_NAME_S SH_NAME_C.c_str()

//Erase previous shared memory and schedule erasure on exit
struct shm_remove {
    shm_remove() {
        bip::shared_memory_object::remove(SH_NAME_S);
    }
    ~shm_remove() {
        bip::shared_memory_object::remove(SH_NAME_S);
    }
};

struct test_data {

    typedef boost::interprocess::interprocess_sharable_mutex mutex_type;
    unsigned long long value;
    //Mutex to protect access to the data
    mutex_type mutex;

    //Condition to wait when the has been written
    boost::interprocess::interprocess_condition_any cond_written;

};

static constexpr auto SLEEP = boost::chrono::microseconds(100000);
static constexpr unsigned long long COUNT = 100;
static constexpr size_t READ_COUNT = 40;

int main(int argc, char** argv) {
    const std::string prog = argv[0];
    const std::string arg = argc > 1 ? argv[1] : "w";
    rtest::MsgCollector COLL;

    int retval = 0;

    try {

        //
        // Writer
        //
        if (arg == "w") {
            COLL << "Starting writer";
            shm_remove remover;
            //Create a shared memory object.
            bip::shared_memory_object shm(bip::create_only         //only create
                    , SH_NAME_S           //name
                    , bip::read_write                //read-write mode
                    );

            //Set size
            shm.truncate(bip::mapped_region::get_page_size());

            //Map the whole shared memory in this process
            bip::mapped_region region(shm                       //What to map
                    , bip::read_write //Map it as read-write
#if defined(FIXED_MAPPING_ADDRESS)
                    , 0, 0, (void*) 0x3f00000000
#endif
                    );

            //Get the address of the mapped region
            void * addr = region.get_address();

            //Construct the shared structure in memory
            test_data * data = new (addr) test_data;
            const boost::chrono::high_resolution_clock::time_point start =
                    boost::chrono::steady_clock::now();
            COLL << boost::format("Start %1%") % start;
            size_t waitCounter = 0;
            for (unsigned long long i = 0; i < COUNT && waitCounter < 5; i++) {
                boost::this_thread::sleep_until(start + (i + 1) * SLEEP);
                {
                    bip::scoped_lock<test_data::mutex_type> lock(data->mutex,bip::try_to_lock);
                    if (lock) {
                        data->value = i;
                        COLL << boost::format("Writing %1%/%2%") % i % COUNT;
                        data->cond_written.notify_all();
                        lock.unlock();
                    } else {
                        COLL << boost::format("Writer waiting") % i % COUNT;
                        waitCounter++;
                        boost::this_thread::sleep_for(boost::chrono::microseconds(2000));
                    }
                }
            }
            COLL << boost::format( "Writer wcount %1%") % waitCounter;
        }

        //
        // Reader
        //
        if (arg[0] == 'r') {
            COLL << boost::format("Starting reader %1%") % arg;
            boost::this_thread::sleep_for(SLEEP/3);
            //Create a shared memory object.
            bip::shared_memory_object shm(bip::open_only         //only open
                    , SH_NAME_S           //name
                    , bip::read_write                //read-write mode
                    );

            //Map the whole shared memory in this process
            bip::mapped_region region(shm                       //What to map
                    , bip::read_write //Map it as read-write
#if defined(FIXED_MAPPING_ADDRESS)
                    , 0, 0, (void*) 0x3f00000000
#endif
                    );

            //Get the address of the mapped region
            void * addr = region.get_address();

            //Construct the shared structure in memory
            test_data * data = (test_data*) addr;

            size_t waitCounter = 0;
            for (size_t i = 0; i < READ_COUNT && waitCounter < 5; i++) {
                {
                    bip::sharable_lock<test_data::mutex_type> lock(data->mutex,
                            bip::defer_lock);
                    lock.timed_lock(boost::posix_time::microsec_clock::local_time()+boost::posix_time::milliseconds(2000));
                    if (lock) {
                        if (data->cond_written.timed_wait(lock,
                                boost::posix_time::microsec_clock::local_time()
                                        + boost::posix_time::microseconds(
                                                200))) {
                            COLL
                                    << boost::format("Read %1% %2%/%3%: %4%")
                                            % arg % i % READ_COUNT
                                            % data->value;
                            lock.unlock();
                        }
                    } else {
                        COLL << boost::format("Waiting %1%") % arg;
                        waitCounter++;
                        //throw std::runtime_error("Waiting");
                        boost::this_thread::sleep_for(
                                boost::chrono::microseconds(2000));
                    }
                }
            }
            bip::shared_memory_object::remove( SH_NAME_S );
            COLL << boost::format( "WCounter %1%: %2%") % arg % waitCounter;
        }

        //
        // Subprocesses
        //
        if (arg == "a") {
            COLL << "Starting subprocesses";

            rtest::SubProc procs;
            procs.add(prog, "w" );
            procs.add(prog, "r1" );
            procs.add(prog, "r2" );

            std::vector<std::string> suboutputs = procs.runAndCollect(COLL);

            for (auto const & i : suboutputs) {
                std::cout << i << std::endl;
            }
        }

    } catch (bip::interprocess_exception const&ex) {
        COLL << boost::format("BIP error: %1%") % ex.what();
        return 1;
    } catch ( std::exception const& e2 ) {
        COLL << boost::format("General error: %1%") % e2.what();
        return 1;
    }
    return retval;
}


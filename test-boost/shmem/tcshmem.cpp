/*
 * tcshmem.cpp
 *
 *  Created on: 15.11.2017
 *      Author: netz
 */

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

namespace bip = boost::interprocess;

rtest::MsgCollector COLL;

typedef bip::string ipstring;

static const ipstring SH_NAME{"myshared"};

//Erase previous shared memory and schedule erasure on exit
struct shm_remove {
	shm_remove() {
		bip::shared_memory_object::remove(SH_NAME.c_str());
	}
	~shm_remove() {
		bip::shared_memory_object::remove(SH_NAME.c_str());
	}
};

struct test_data {
	unsigned long long value;
	//Mutex to protect access to the data
	boost::interprocess::interprocess_sharable_mutex mutex;

	//Condition to wait when the has been written
	boost::interprocess::interprocess_condition_any cond_written;

};

static constexpr auto SLEEP = boost::chrono::microseconds(100000);
static constexpr unsigned long long COUNT = 200;
static constexpr size_t READ_COUNT = 40;

int main(int argc, char** argv) {
	const std::string arg = argc > 1 ? argv[1] : "w";

	try {

		//
		// Writer
		//
		if (arg == "w") {
			COLL << "Starting writer";
			shm_remove remover;
			//Create a shared memory object.
			bip::shared_memory_object shm(bip::create_only         //only create
					, SH_NAME.c_str()           //name
					, bip::read_write                //read-write mode
					);

			//Set size
			shm.truncate(sizeof(test_data));

			//Map the whole shared memory in this process
			bip::mapped_region region(shm                       //What to map
					, bip::read_write //Map it as read-write
					);

			//Get the address of the mapped region
			void * addr = region.get_address();

			//Construct the shared structure in memory
			test_data * data = new (addr) test_data;
			const boost::chrono::high_resolution_clock::time_point start = boost::chrono::steady_clock::now();
			COLL << boost::format( "Start %1%" ) % start;
			for (unsigned long long i = 0; i < COUNT; i++) {
				boost::this_thread::sleep_until(start + (i+1)*SLEEP);
				{
					bip::scoped_lock<bip::interprocess_sharable_mutex> lock(data->mutex);
					data->value = i;
					COLL << boost::format( "Writing %1%/%2%" ) % i % COUNT;
					data->cond_written.notify_all();
				}
			}
		}

		//
		// Writer
		//
		if (arg == "r") {
			COLL << "Starting reader";
			//Create a shared memory object.
			bip::shared_memory_object shm(bip::open_only         //only open
					, SH_NAME.c_str()           //name
					, bip::read_write                //read-write mode
					);

			//Map the whole shared memory in this process
			bip::mapped_region region(shm                       //What to map
					, bip::read_write //Map it as read-write
					);

			//Get the address of the mapped region
			void * addr = region.get_address();

			//Construct the shared structure in memory
			test_data * data = new (addr) test_data;

			for (size_t i = 0; i < READ_COUNT; i++) {
				{
					bip::sharable_lock<bip::interprocess_sharable_mutex> lock(data->mutex);
					data->cond_written.wait(lock);
					COLL << boost::format("Read %1%/%2%: %3%") % i % READ_COUNT % data->value;
					lock.unlock();
				}
			}

		}
	} catch (bip::interprocess_exception &ex) {
		COLL << boost::format("BIP error: %1%") % ex.what();
	}
	COLL.finalize();
	return 0;
}


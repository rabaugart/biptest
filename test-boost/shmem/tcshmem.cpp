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
#include <boost/process.hpp>
#include <boost/format.hpp>
#include <boost/asio.hpp>
#include "test/MsgCollector.h"

namespace bip = boost::interprocess;


#if 0
typedef bip::string ipstring;
static const ipstring SH_NAME_C{"myshared"};
#define SH_NAME_S SH_NAME_C.c_str()
#else
#define SH_NAME_S "myshared"
#endif

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
	unsigned long long value;
	//Mutex to protect access to the data
	boost::interprocess::interprocess_sharable_mutex mutex;

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
		// Reader
		//
		if (arg[0] == 'r') {
			COLL << boost::format("Starting reader %1%") % arg;
			//Create a shared memory object.
			bip::shared_memory_object shm(bip::open_only         //only open
					, SH_NAME_S           //name
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
					bip::scoped_lock<bip::interprocess_sharable_mutex> lock(data->mutex);
					data->cond_written.wait(lock);
					COLL << boost::format("Read %1% %2%/%3%: %4%") % arg % i % READ_COUNT % data->value;
				}
			}

		}

		//
		// Subprocesses
		//
		if ( arg == "a" ) {
			COLL << "Starting subprocesses";

			namespace bp = boost::process;

			boost::asio::io_service ios;

			std::future<std::string> dataw;
			bp::child cw(prog, "w", //set the input
			        bp::std_in.close(),
			        bp::std_out > dataw, //so it can be written without anything
			        bp::std_err > bp::null,
			        ios);

			std::future<std::string> datar;
			bp::child cr(prog, "r1", //set the input
			        bp::std_in.close(),
			        bp::std_out > datar, //so it can be written without anything
			        bp::std_err > bp::null,
			        ios);

			ios.run(); //this will actually block until the compiler is finished

			cw.wait();
			cr.wait();

			std::cout << dataw.get() << std::endl;
			std::cout << datar.get() << std::endl;
		}

	} catch (bip::interprocess_exception &ex) {
		COLL << boost::format("BIP error: %1%") % ex.what();
	}
	return 0;
}


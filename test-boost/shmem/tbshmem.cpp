/*
 * tbshmem.cpp
 *
 *  Created on: 25.10.2017
 *      Author: netz
 */

#include <iostream>
#include <sstream>
#include <cstdio>

#include <boost/chrono.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/optional.hpp>
#include <boost/interprocess/sync/sharable_lock.hpp>

#include "shmem/ShmWriter.h"
#include "shmem/ShmReader.h"

#include "test/TestDataA.h"
#include "test/MsgCollector.h"

using namespace rshm;
namespace bip = boost::interprocess;

template<typename SDATA>
class Generator {
public:
	const SDATA& next();
	const SDATA& current() {
		return data;
	}
protected:
	SDATA data;
};

template<>
const TestDataA& Generator<TestDataA>::next() {
	data.b += 1;
	return data;
}

static constexpr auto SLEEP = boost::chrono::microseconds(20000);
rtest::MsgCollector COLL;

template<typename DATA, typename ID = ShmDefaultId>
struct Writer: ShmWriter<DATA, ID> {

	typedef DATA data_type;

	Writer(ShmSegment& seg, size_t _niter) :
			ShmWriter<DATA>(seg), th(std::ref(*this)), niter(_niter), i(0) {
		COLL << "WR created";
	}

	void operator()() {
		do {
			const data_type& d = gen.next();
			std::ostringstream os;
			os << "WR count " << i << " " << d;
			COLL << os.str();
			ShmWriter<data_type, ID>::set(d);
			boost::this_thread::sleep_for(SLEEP);
		} while (i++ < niter);
	}

	void join() {
		th.join();
	}

	boost::thread th;
	const size_t niter;
	size_t i;
	Generator<data_type> gen;
};

template<typename DATA, typename ID = ShmDefaultId>
struct Reader: ShmReader<DATA, ID> {

	typedef DATA data_type;

	Reader(ShmSegment& seg, const std::string& name_, size_t _niter) :
			ShmReader<DATA>(seg), th(std::ref(*this)), name(name_), niter(_niter), i(0) {
		COLL << "Reader " + name + " created";
	}

	void operator()() {
		try {
			do {
				typename ShmReader<DATA, ID>::time_point_type dtime;
				{
					bip::scoped_lock<bip::interprocess_sharable_mutex> lck(
							ShmReader<DATA, ID>::fptr->mutex);
					ShmReader<DATA, ID>::fptr->cond.wait(lck);
					data = ShmReader<DATA, ID>::getNoLock();
					dtime = ShmReader<DATA, ID>::fptr->timestamp;
				}
				std::ostringstream os;
				os << name << " count " << i++ << " " << data << " dt:"
						<< std::chrono::duration_cast<std::chrono::microseconds>(
								ShmReader<DATA, ID>::now() - dtime).count()
						<< " μsec";
				COLL << os.str();
			} while (ShmReader<DATA, ID>::getCounter() < niter / 2);
		} catch (const std::exception& e) {
			std::ostringstream os;
			os << "ERROR: " << name << ": " << e.what();
			COLL << os.str();
		}
	}

	void join() {
		th.join();
	}

	boost::thread th;
	std::string name;
	data_type data;
	const size_t niter;
	size_t i;
};

int main(int argc, char** argv) {

	ShmSegment seg;
	const size_t niter = 20;

	const std::string arg = argc > 1 ? argv[1] : "a";

	if ( arg == "r1" || arg == "r2" ) {
		Reader<TestDataA> re {seg, arg, niter};
		re.join();
		return 0;
	}

	if ( arg == "w" ) {
		Writer<TestDataA> wr {seg, niter};
		wr.join();
		return 0;
	}

	bip::shared_memory_object::remove(ShmSegment::SHM_FRAME_NAME);

	// a: Start readers and writers in this process
	Reader<TestDataA> re {seg, "re1", niter};

	boost::this_thread::sleep_for( SLEEP/5 );
	Reader<TestDataA> re2 {seg, "re2", niter};

	Writer<TestDataA> wr {seg, niter};

	wr.join();
	re.join();
	re2.join();
	COLL.finalize();
	bip::shared_memory_object::remove(ShmSegment::SHM_FRAME_NAME);
	if ( wr.gen.current().b == re.data.b ) {
		throw std::runtime_error( "Invalid counter" );
	}

}

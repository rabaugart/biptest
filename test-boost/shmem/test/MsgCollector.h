/*
 * MsgCollector.h
 *
 *  Created on: 26.10.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_MSGCOLLECTOR_H_
#define SHMEM_TEST_MSGCOLLECTOR_H_

#include <mutex>
#include <string>
#include <condition_variable>
#include <queue>
#include <thread>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>

namespace rtest {

class MsgCollector {
public:
	MsgCollector();

	void operator()();

	void finalize();

	MsgCollector& operator<<( const std::string& s );

	MsgCollector& operator<<( const boost::format& bf );

protected:
	std::mutex mtx;
	std::condition_variable cond;
	std::queue<std::pair<boost::posix_time::ptime,std::string>> mqueue;
	std::thread outThread;
};

} /* namespace rtest */

#endif /* SHMEM_TEST_MSGCOLLECTOR_H_ */

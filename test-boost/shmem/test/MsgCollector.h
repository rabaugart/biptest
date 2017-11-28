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

    struct use_queue_t {};
    static constexpr use_queue_t use_queue{};

	MsgCollector(use_queue_t);
    MsgCollector();
	~MsgCollector();

	void operator()();

	void finalize();

	MsgCollector& operator<<( const std::string& s );

	MsgCollector& operator<<( const boost::format& bf );

protected:
	bool const useQueue;
	std::mutex mtx;
	std::condition_variable cond;
	std::queue<std::pair<boost::posix_time::ptime,std::string>> mqueue;
	std::thread outThread;
	bool running;
};

} /* namespace rtest */

#endif /* SHMEM_TEST_MSGCOLLECTOR_H_ */

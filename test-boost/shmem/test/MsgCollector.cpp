/*
 * MsgCollector.cpp
 *
 *  Created on: 26.10.2017
 *      Author: netz
 */
#include <iostream>
#include "MsgCollector.h"

namespace rtest {

MsgCollector::MsgCollector() : outThread(std::ref(*this)), running(true) {
	outThread.detach();
}

void MsgCollector::finalize() {
	if (running) {
		running = false;
		*this << "Finished";
		std::unique_lock<std::mutex> lck(mtx);
		if (!mqueue.empty()) {
			const auto msg = mqueue.front();
			mqueue.pop();
			std::cout << msg.first << " " << msg.second << "\n";
		}
	}
}

void MsgCollector::operator ()() {
	while (running) {
		std::unique_lock<std::mutex> lck(mtx);
		cond.wait(lck);
		if ( !mqueue.empty() ) {
			const auto msg = mqueue.front();
			mqueue.pop();
			std::cout << msg.first << " " << msg.second << std::endl;
		}
	}
}

MsgCollector& MsgCollector::operator<<( const std::string& s )
{
	std::unique_lock<std::mutex> lck(mtx);
	mqueue.push(std::make_pair(boost::posix_time::microsec_clock::local_time(),s));
	cond.notify_all();
	return *this;
}

MsgCollector::~MsgCollector() {
	finalize();
}

MsgCollector& MsgCollector::operator<<( const boost::format& bf )
{
	*this << bf.str();
	return *this;
}

} /* namespace rtest */

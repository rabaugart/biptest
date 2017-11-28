/*
 * MsgCollector.cpp
 *
 *  Created on: 26.10.2017
 *      Author: netz
 */
#include <iostream>
#include "MsgCollector.h"

namespace rtest {

MsgCollector::MsgCollector(use_queue_t) :
                        useQueue(true), outThread(std::ref(*this)), running(true) {
}

MsgCollector::MsgCollector() :
                        useQueue(false), outThread(), running(false) {
}

void MsgCollector::finalize() {
    if (useQueue && running) {
        running = false;
        *this << "Finished";
        outThread.join();
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
        std::unique_lock < std::mutex > lck(mtx);
        cond.wait(lck);
        if (!mqueue.empty()) {
            const auto msg = mqueue.front();
            mqueue.pop();
            std::cout << msg.first << " " << msg.second << std::endl;
        }
    }
}

MsgCollector& MsgCollector::operator<<(const std::string& s) {
    if (!useQueue) {
        std::cout << boost::posix_time::microsec_clock::local_time() << " " << s
                << std::endl;
    } else {
        std::unique_lock < std::mutex > lck(mtx);
        mqueue.push(std::make_pair(boost::posix_time::microsec_clock::local_time(), s));
        cond.notify_all();
    }
    return *this;
}

MsgCollector::~MsgCollector() {
    finalize();
}

MsgCollector& MsgCollector::operator<<(const boost::format& bf) {
    *this << bf.str();
    return *this;
}

} /* namespace rtest */

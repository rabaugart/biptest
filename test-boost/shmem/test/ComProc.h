/*
 * ComProc.h
 *
 *  Created on: 29.11.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_COMPROC_H_
#define SHMEM_TEST_COMPROC_H_

#include <future>
#include <string>
#include <boost/asio.hpp>
#include <boost/process.hpp>
#include <boost/regex.hpp>

#include "MsgCollector.h"

namespace rtest {

struct ComProc {
    ComProc(const std::string & prog, const std::string& arg,
            boost::asio::io_service& ios);

    void append(std::vector<std::string>& v);

    int wait();

    std::future<std::string> data;
    boost::process::child proc;
    const std::string name;
    const boost::regex rx;
};

} /* namespace rtest */

#endif /* SHMEM_TEST_COMPROC_H_ */

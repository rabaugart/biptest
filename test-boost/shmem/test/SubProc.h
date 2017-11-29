/*
 * SubProc.h
 *
 *  Created on: 29.11.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_SUBPROC_H_
#define SHMEM_TEST_SUBPROC_H_

#include <string>
#include <vector>
#include <boost/asio.hpp>
#include "ComProc.h"
#include "MsgCollector.h"

namespace rtest {

struct SubProc : public std::vector<ComProc> {

    void add( std::string const & prog, std::string const & arg );

    std::vector<std::string> runAndCollect(MsgCollector & coll);

    boost::asio::io_service ios;
};
} /* namespace rtest */

#endif /* SHMEM_TEST_SUBPROC_H_ */

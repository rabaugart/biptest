/*
 * ComProc.cpp
 *
 *  Created on: 29.11.2017
 *      Author: netz
 */

#include <boost/algorithm/string/regex.hpp>

#include <test/ComProc.h>

namespace rtest {

namespace bp = boost::process;

ComProc::ComProc(const std::string & prog, const std::string& arg,
        boost::asio::io_service& ios) :
        proc(prog, arg, bp::std_in.close(), bp::std_out > data,
                bp::std_err > bp::null, ios), name(arg), rx("\n") {
}

void ComProc::append(std::vector<std::string>& v) {
     std::vector<std::string> rr;
     boost::algorithm::split_regex(rr, data.get(), rx);
     std::for_each(rr.begin(), rr.end(),
             [&v]( const std::string &i) {v.push_back(i);});
 }

 int ComProc::wait() {
     proc.wait();
     return proc.exit_code();
 }

} /* namespace rtest */

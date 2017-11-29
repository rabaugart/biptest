/*
 * SubProc.cpp
 *
 *  Created on: 29.11.2017
 *      Author: netz
 */

#include <boost/format.hpp>
#include <test/SubProc.h>

namespace rtest {

namespace bp = boost::process;

void SubProc::add( std::string const & prog, std::string const & arg ) {
    emplace_back( prog, arg, ios );
}

std::vector<std::string> SubProc::runAndCollect(rtest::MsgCollector & coll) {
    ios.run(); //this will actually block until the subprocs are finished
    std::vector<std::string> outputs;
    for (auto& i : *this) {
        try {
            int const ec = i.wait();
            if (ec) {
                coll
                        << boost::format("Error %1%: %2%") % i.name
                                % ec;
            }
        } catch (bp::process_error const & err) {
            coll
                    << boost::format("Process error when waiting %1%")
                            % err.what();
        }
        i.append(outputs);
    }

    std::sort(outputs.begin(), outputs.end());
    return outputs;
}

} /* namespace rtest */

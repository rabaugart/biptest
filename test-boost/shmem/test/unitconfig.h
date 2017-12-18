/*
 * unitconfig.h
 *
 *  Created on: 15.12.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_UNITCONFIG_H_
#define SHMEM_TEST_UNITCONFIG_H_

#include <string>
#include <vector>

#include <boost/process.hpp>

namespace utest {

/// Configuration of subprocesses in unittests
struct unit_config {
    unit_config();

    std::string toString() const;

    static unit_config fromString(std::string const& cfgstr);

    std::string comp_name;
    size_t niter;

    /// Maximum duration of the tests in ms
    size_t duration;

    /// Timeout in timed_wait in ms
    size_t timeout;

    /// Period duration in us
    size_t period;
};

/// An adapter for a child process with predefined command line arguments
struct process {

    static std::string const runner;
    static std::string const runnerpath;

    process( std::string const& com, unit_config const& cfg, std::string const& id );

    bool wait();

    const std::string command;
    const unit_config config;
    const std::string id;

    boost::process::child ch;
};

/// A vector of child processes
struct process_vec : public std::vector<process> {

    virtual ~process_vec() {}

    /// Add a new child process and start it immediately
    void add( std::string const& com, unit_config const& cfg, std::string const& id="nn" );

    /**
     * Check the result of all processes
     *
     * Returns true if every process exited without error
     */
    bool waitall();

    std::string message;
};

}
#endif /* SHMEM_TEST_UNITCONFIG_H_ */

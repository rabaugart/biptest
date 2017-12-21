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
#include <functional>

#include <boost/process.hpp>
#include <boost/asio.hpp>

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

    /// Path to the binary directory
    std::string bin_path;
};

struct test_result {
    test_result() :
            name { "nn" }, n_loop(0), n_timeouts(0), n_no_segment(0), n_counter_errors(
                    0), counter_error_pos(std::numeric_limits<size_t>::max()), first_counter(
                    std::numeric_limits<size_t>::max()), last_counter(
                    std::numeric_limits<size_t>::max()) {
    }

    std::string toString() const;

    static test_result fromString(std::string const& cfgstr);

    std::string name;

    size_t n_loop;
    size_t n_timeouts;
    size_t n_no_segment;
    size_t n_counter_errors;
    size_t counter_error_pos;
    size_t first_counter;
    size_t last_counter;
};

std::ostream& operator <<(std::ostream& os, test_result const& tr);

/// An adapter for a child process with predefined command line arguments
struct process {

    static std::string const runner;

    process(std::string const& com, unit_config const& cfg,
            std::string const& id, boost::asio::io_service& io);

    /**
     * Wait for the process to finish and process the result
     *
     * @return true if the process returned without error
     */
    bool wait();

    const std::string command;
    const unit_config config;
    const std::string id;

    std::string message;
    std::vector<std::string> output;
    std::future<std::string> data;
    boost::process::child ch;

    test_result result;
};

/// A vector of child processes
struct process_vec: public std::vector<process> {

    virtual ~process_vec() {
    }

    typedef std::function<std::string(size_t)> name_func;

    void add(std::string const& com, unit_config const& cfg);

    void add(std::string const& com, unit_config const& cfg, name_func f);

    /// Add a new child process and start it immediately
    void add(std::string const& com, unit_config const& cfg,
            std::string const& id);

    /**
     * Check the result of all processes
     *
     * Returns true if every process exited without error
     */
    bool waitall();

    boost::asio::io_service ios;
    std::string message;
};

struct COM {
    static constexpr char const* READER="rs";
    static constexpr char const* WRITER="as";
};
}
#endif /* SHMEM_TEST_UNITCONFIG_H_ */

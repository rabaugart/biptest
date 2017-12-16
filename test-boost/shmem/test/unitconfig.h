/*
 * unitconfig.h
 *
 *  Created on: 15.12.2017
 *      Author: netz
 */

#ifndef SHMEM_TEST_UNITCONFIG_H_
#define SHMEM_TEST_UNITCONFIG_H_

#include <string>

struct unit_config {
    unit_config();

    std::string toString() const;

    static unit_config fromString(std::string const& cfgstr);

    std::string comp_name;
    size_t niter;

    /// Maximum duration of the tests in seconds
    size_t duration;

    /// Timeout in timed_wait in ms
    size_t timeout;

    /// Period duration in us
    size_t period;
};

#endif /* SHMEM_TEST_UNITCONFIG_H_ */

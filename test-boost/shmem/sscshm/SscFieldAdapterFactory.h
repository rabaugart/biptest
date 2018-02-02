/*
 * SscFieldAdapterFactory.h
 *
 *  Created on: Feb 1, 2018
 *      Author: ran
 */

#ifndef SSC_SHMEM_SSCSHM_SSCFIELDADAPTERFACTORY_H_
#define SSC_SHMEM_SSCSHM_SSCFIELDADAPTERFACTORY_H_

#include <string>
#include <memory>
#include <iostream>

#include "rashm/FieldAdapter.h"
#include "ssc_signal_values.h"

namespace ran {
namespace ssc {

class SscFieldAdapterFactory {
public:

	typedef rashm::FieldAdapter<signal_values> adapter_t;
	typedef std::shared_ptr<adapter_t> adapter_ptr;
	typedef typename adapter_t::ValueFrame signal_frame_t;

	SscFieldAdapterFactory();
	virtual ~SscFieldAdapterFactory();

	/// Create an adapter with 'segment:key:format'
	adapter_ptr makeAdapter( std::string const& segkey );

	adapter_ptr makeAdapter( std::string const& segment, std::string const& key, std::string const& format="" );

	void showFields( std::ostream& os );

	void stop();

protected:

	struct Impl;

	std::unique_ptr<Impl> pimpl;
};

}
}

#endif /* SSC_SHMEM_SSCSHM_SSCFIELDADAPTERFACTORY_H_ */

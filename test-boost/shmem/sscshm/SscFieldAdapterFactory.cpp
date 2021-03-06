/*
 * SscFieldAdapterFactory.cpp
 *
 *  Created on: Feb 1, 2018
 *      Author: ran
 */

#include <sscshm/SscFieldAdapterFactory.h>

#include <thread>
#include <vector>

#include <boost/algorithm/string.hpp>

#include "rashm/SegmentReader.h"
#include "rashm/CompMap.h"
#include "rashm/FieldAdapter.h"
#include "rashm/FieldAdapterFactory.h"
#include "all_ssc_data_types.h"
#include "ssc_signal_values.h"

namespace ran {
namespace ssc {

typedef rashm::FieldAdapter<signal_values> ssc_adapter_t;
typedef typename ssc_adapter_t::ValueFrame signal_frame_t;
typedef std::shared_ptr<ssc_adapter_t> ssc_adapter_ptr_t;

class FieldReaderBase {
public:

    virtual ssc_adapter_ptr_t makeAdapter(std::string const& key,
            std::string const& format) = 0;

    virtual void stop() = 0;
    virtual void wait() = 0;

    virtual void showFields(std::ostream& os) = 0;
    virtual std::vector<ssc_adapter_t::FieldDescriptor> fieldDescriptors() = 0;
};

struct FieldReaderConfig {
};

template<typename DATA, typename ID>
class FieldReader: public FieldReaderBase, public std::thread {
public:

    typedef rashm::FieldAdapterFactory<DATA, signal_values> adapter_factory_t;
    typedef typename adapter_factory_t::adapter_ptr_t adapter_ptr_t;

    FieldReader(FieldReaderConfig const&) :
            std::thread(std::ref(*this)), running(true) {
        //detach();
    }

    // Todo: Destructor calling stop and wait

    virtual ssc_adapter_ptr_t makeAdapter(std::string const& key,
            std::string const& format) {
        // Todo: Here the reader should be started
        return factory.makeAdapter(key, format);
    }

    virtual void stop() {
        running = false;
    }

    virtual void wait() {
        if (joinable()) {
            join();
        }
    }

    void operator()() {
        // Todo: The reader should not be started unless an adapter is listening on it
        rashm::SegmentReader<DATA, ID> reader;
        while (running) {
            try {
                factory = reader.timed_wait_for(
                        boost::posix_time::microsec { 3000 * 1000 });
            } catch (rashm::timeout_error const&) {
                std::cout << "Timeout " << reader.segmentName() << std::endl;
                factory.setValid(false);
            } catch (...) {
                std::cout << "Stopping because of unknown exception "
                        << reader.segmentName() << std::endl;
                running = false;
            }
        }
        std::cout << "Stopping " << reader.segmentName() << std::endl;
    }

    void showFields(std::ostream& os) {
        factory.listFields(os);
    }

    std::vector<ssc_adapter_t::FieldDescriptor> fieldDescriptors() {
        return factory.fieldDescriptors();
    }

protected:

    adapter_factory_t factory;
    bool running;
};

struct FieldReaderFactory {
    typedef FieldReaderBase type;
    typedef std::shared_ptr<type> value_type;
    typedef FieldReaderConfig config_type;

    // Todo: We create a reader for every segment, even if it is not required
    template<typename DATA, typename ID>
    static value_type make(config_type const& cfg) {
        return std::make_shared<FieldReader<DATA, ID>>(cfg);
    }
};

typedef rashm::CompMap<FieldReaderFactory> ssc_comp_map_t;

// Todo: Monitor the different threads/FieldReader, use futures
ssc_comp_map_t makeMap() {
    return rashm::makeMap<data_vector_t, FieldReaderFactory>(
            FieldReaderConfig());
}

struct SscFieldAdapterFactory::Impl {

    Impl() :
            map(makeMap()) {
    }

    ssc_comp_map_t map;
};

SscFieldAdapterFactory::SscFieldAdapterFactory() :
        pimpl(std::make_unique<Impl>()) {
    // TODO Auto-generated constructor stub

}

SscFieldAdapterFactory::~SscFieldAdapterFactory() {
    stop();
}

SscFieldAdapterFactory::adapter_ptr SscFieldAdapterFactory::makeAdapter(
        std::string const& segkey) {
    std::vector<std::string> spvec;
    boost::split(spvec, segkey, boost::is_any_of(":"),
            boost::token_compress_on);
    if (spvec.size() == 2) {
        return makeAdapter(spvec[0], spvec[1]);
    }
    if (spvec.size() == 3) {
        return makeAdapter(spvec[0], spvec[1], spvec[2]);
    }
    throw std::runtime_error("Invalid number of elements in segkey");
}

SscFieldAdapterFactory::adapter_ptr SscFieldAdapterFactory::makeAdapter(
        std::string const& segment, std::string const& key,
        std::string const& format) {
    return pimpl->map.at(segment)->makeAdapter(key, format);
}

void SscFieldAdapterFactory::showFields(std::ostream& os) {
    for (auto & i : pimpl->map) {
        os << "Segment " << i.first << std::endl;
        i.second->showFields(os);
    }
}

std::vector<std::string> SscFieldAdapterFactory::segmentNames() const {
    std::vector<std::string> ret;
    for (auto const & i : pimpl->map) {
        ret.push_back(i.first);
    }
    return ret;
}

std::vector<SscFieldAdapterFactory::adapter_t::FieldDescriptor> SscFieldAdapterFactory::fieldDescriptors(
        std::string const& segmentName) const {
    auto & seg = pimpl->map.at(segmentName);
    return seg->fieldDescriptors();
}

void SscFieldAdapterFactory::stop() {
    std::cout << "Stopping factory" << std::endl;
    for (auto & i : pimpl->map) {
        i.second->stop();
    }
    for (auto & i : pimpl->map) {
        i.second->wait();
    }
}

}
}

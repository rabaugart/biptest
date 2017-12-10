/*
 * tcomp.cpp
 *
 *  Created on: 05.12.2017
 *      Author: netz
 */

#include <map>
#include <memory>
#include <thread>
#include <sstream>

#include <boost/program_options.hpp>
#include <boost/chrono.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/asio.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/variant.hpp>
#include <boost/serialization/variant.hpp>

#include <boost/bind.hpp>

#include "rashm/Segment.h"
#include "rashm/SegmentWriter.h"
#include "rashm/SegmentReader.h"
#include "rashm/CompMap.h"
#include "rashm/data_functors.h"
#include "rashm/Packet.h"

#include "test/all_data_types.h"
#include "test/TestGenerator.h"

struct NetConfig {
    size_t duration;
    size_t timeout;
    std::string address;
    unsigned short port;
    size_t period;
};

class SenderBase {
public:
    virtual ~SenderBase() {
    }
    virtual void start() = 0;
    virtual void join() = 0;
    virtual void removeSegment() = 0;
};

//
// Sender
//
template<typename DATA, typename ID>
class Sender: public SenderBase {
public:

    Sender(NetConfig cfg_) :
            cfg(cfg_) {
    }

    virtual ~Sender() {
        if (th.joinable()) {
            th.join();
        }
    }

    virtual void start() {
        BOOST_LOG_TRIVIAL(info)<< "starting sender " << rashm::DataIdTraits<DATA, ID>::name();

        th = std::move(std::thread(std::ref(*this)));
    }

    void operator()() {
        boost::chrono::time_point<boost::chrono::system_clock> const start =
        boost::chrono::system_clock::now();

        while (boost::chrono::system_clock::now() - start
                < boost::chrono::seconds(cfg.duration)) {
            try {
                rashm::SegmentReader<DATA, ID> sr(
                        boost::interprocess::open_only);
                boost::asio::io_service io;

                boost::asio::ip::address const to {boost::asio::ip::address::from_string(cfg.address)};
                boost::asio::ip::udp::endpoint const ep {to,cfg.port};
                boost::asio::ip::udp::socket sock {io,ep.protocol()};

                BOOST_LOG_TRIVIAL(info) << "shm address " << sr.address();

                boost::posix_time::microseconds timeout(cfg.timeout * 1000);

                while (boost::chrono::system_clock::now() - start
                        < boost::chrono::seconds(cfg.duration)) {

                    try {
                        typedef rashm::apply_all_data_ids<data_vector_t, rashm::packet_functor_t>::type all_packets_t;

                        typedef boost::make_variant_over<all_packets_t>::type all_packet_variant_t;

                        DATA d = sr.timed_wait_for(timeout);
                        BOOST_LOG_TRIVIAL(debug) << "read " << sr.headerTime()
                        << sr.lastAge().total_microseconds() << "us "
                        << d;

                        rashm::Packet<DATA,ID> pa {sr.head(),d}; // Todo: protect head by lock

                        std::ostringstream os;

                        {
                            boost::archive::text_oarchive oa(os); // Todo: use bin archive
                            all_packet_variant_t v {pa};
                            oa << v;
                        }

                        std::string buf {os.str()};
                        sock.send_to( boost::asio::buffer(buf), ep );

                    } catch (std::runtime_error const & e) { // Todo: create timeout exception
                        BOOST_LOG_TRIVIAL(info) << "timeout (last "
                        << sr.lastReceptionTime() << "/"
                        << sr.headerTime() << ")" << e.what();
                    }

                }
            } catch (boost::interprocess::interprocess_exception const & e) {
                if (e.get_error_code()
                        == boost::interprocess::not_found_error) {
                    BOOST_LOG_TRIVIAL(info) << "no segment";
                    std::this_thread::sleep_for(
                            std::chrono::milliseconds(1000));
                } else {
                    throw;
                }
            }
        }

    }

    void join() {
        if (th.joinable()) {
            th.join();
        }
    }

    virtual void removeSegment() {
        rashm::Segment<DATA, ID>::removeSegment();
    }

    NetConfig cfg;
    std::thread th;
};

struct SenderFactory {
    typedef SenderBase type;
    typedef std::shared_ptr<type> value_type;
    typedef NetConfig config_type;

    template<typename DATA, typename ID>
    static value_type make(config_type const& cfg) {
        return std::make_shared<Sender<DATA, ID>>(cfg);
    }
};

template<class VISITOR>
class udp_server {
public:
    udp_server(NetConfig const& cfg, boost::asio::io_service& io_service) :
            socket_(io_service,
                    boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(),
                            cfg.port)) {
        start_receive();
    }

private:
    void start_receive() {
        socket_.async_receive_from(boost::asio::buffer(recv_buffer_),
                remote_endpoint_,
                boost::bind(&udp_server::handle_receive, this,
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
    }

    void handle_receive(const boost::system::error_code& error,
            std::size_t size) {
        if (!error || error == boost::asio::error::message_size) {
            typedef rashm::apply_all_data_ids<data_vector_t,
                    rashm::packet_functor_t>::type all_packets_t;

            typedef boost::make_variant_over<all_packets_t>::type all_packet_variant_t;

            std::string const in(&(recv_buffer_[0]), size);

            std::istringstream is(in);
            boost::archive::text_iarchive ia(is);
            all_packet_variant_t p2;
            ia >> p2;
            boost::apply_visitor(VISITOR(), p2);

            start_receive();
        }
    }

    boost::asio::ip::udp::socket socket_;
    boost::asio::ip::udp::endpoint remote_endpoint_;
    boost::array<char, 10000> recv_buffer_; // Todo: fix buffer size
};

struct MappingVisitor {

    template<typename P>
    void operator()(P const& p) const;

    template<typename ID>
    void operator()(rashm::Packet<TestDataA, ID> const& p) const {
        BOOST_LOG_TRIVIAL(debug)<< "received TA " << p.name() << " " << p.head.timestamp;
        rashm::SegmentWriter<TestDataA,TIdA> sw; // Todo: Expensive construction
        sw = p.data;
    }

    template<typename ID>
    void operator()( rashm::Packet<TestDataB,ID> const& p ) const {
        BOOST_LOG_TRIVIAL(debug) << "received TB " << p.name() << " " << p.head.timestamp;
        rashm::SegmentWriter<TestDataB,TIdB1> sw;
        sw = p.data;
    }
};

struct IdentityVisitor {

    template<typename P>
    void operator()(P const& p) const;

    template<typename DATA, typename ID>
    void operator()(rashm::Packet<DATA, ID> const& p) const {
        BOOST_LOG_TRIVIAL(debug)<< "received (a) " << p.name() << " " << p.head.timestamp;
        rashm::SegmentWriter<DATA,ID> sw; // Todo: Expensive construction
        sw = p.data;
    }

};

template<typename VISITOR>
static void receive(NetConfig const& cfg) {
    boost::asio::io_service io;

    udp_server<VISITOR> srv { cfg, io };

    boost::asio::deadline_timer timer { io, boost::posix_time::seconds(
            cfg.duration) };
    timer.async_wait([&io]( boost::system::error_code const& ) {io.stop();});

    io.run();
}

int main(int argc, char** argv) {

    NetConfig cfg;

    std::string compName { "" };
    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()("help,h", "produce help message")("duration,d",
            po::value<size_t>(&(cfg.duration))->default_value(5),
            "duration in seconds")("timeout,t",
            po::value<size_t>(&(cfg.timeout))->default_value(1000),
            "timeout in milliseconds")("port,p",
            po::value<unsigned short>(&(cfg.port))->default_value(9090),
            "Period in milliseconds")("address,a",
            po::value<std::string>(&(cfg.address))->default_value("127.0.0.1"),
            "ip address")("sender,s", po::value<std::string>(&compName),
            "start sender by name")("sall", "start all senders")("rall", "receive all messages")("recv,r",
            "start mapping receiver")("quiet,q", "show only errors");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\nComponents:\n";
        typedef SenderFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);
        for (auto const& i : map) {
            std::cout << i.first << std::endl;
        }
        return 1;
    }

    if (vm.count("quiet")) {
        boost::log::core::get()->set_filter(
                boost::log::trivial::severity > boost::log::trivial::info);
    }

    if (vm.count("sall")) {
        BOOST_LOG_TRIVIAL(info) << "Starting all senders";
        typedef SenderFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);

        for (auto& i : map) {
            i.second->start();
        }

        for (auto& i : map) {
            i.second->join();
        }
        return 0;
    }

    if (vm.count("sender")) {
        typedef SenderFactory fac_t;

        rashm::CompMap<fac_t> const map = rashm::makeMap<data_vector_t, fac_t>(
                cfg);

        map.at(compName)->start();
        map.at(compName)->join();
        return 0;
    }

    if (vm.count("recv")) {
        BOOST_LOG_TRIVIAL(info) << "Receiving all senders";

        receive<MappingVisitor>(cfg);
        return 0;
    }

    if (vm.count("rall")) {

        receive<IdentityVisitor>(cfg);
        return 0;
    }

    return 0;
}


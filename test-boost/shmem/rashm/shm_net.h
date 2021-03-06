/*
 * shm_net.h
 *
 *  Created on: 02.02.2018
 *      Author: netz
 */

#ifndef SHMEM_RASHM_SHM_NET_H_
#define SHMEM_RASHM_SHM_NET_H_

#include <boost/log/trivial.hpp>
#include <boost/chrono.hpp>
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

namespace rashm {

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
template<typename DATAVEC, typename DATA, typename ID>
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
        BOOST_LOG_TRIVIAL(info) << "starting sender "
                << rashm::DataIdTraits<DATA, ID>::name();

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

                boost::asio::ip::address const to {
                        boost::asio::ip::address::from_string(cfg.address) };
                boost::asio::ip::udp::endpoint const ep { to, cfg.port };
                boost::asio::ip::udp::socket sock { io, ep.protocol() };

                BOOST_LOG_TRIVIAL(info) << "shm address " << sr.address();

                boost::posix_time::microseconds timeout(cfg.timeout * 1000);

                while (boost::chrono::system_clock::now() - start
                        < boost::chrono::seconds(cfg.duration)) {

                    try {
                        typedef typename rashm::apply_all_data_ids<DATAVEC,
                                rashm::packet_functor_t>::type all_packets_t;

                        typedef typename boost::make_variant_over<all_packets_t>::type all_packet_variant_t;

                        DATA d = sr.timed_wait_for(timeout);
                        BOOST_LOG_TRIVIAL(debug) << "read " << sr.headerTime()
                                << " " << sr.lastAge().total_microseconds()
                                << "us " << d;

                        rashm::Packet<DATA, ID> pa { sr.head(), d }; // Todo: protect head by lock

                        std::ostringstream os;

                        {
                            boost::archive::text_oarchive oa(os); // Todo: use bin archive
                            all_packet_variant_t v { pa };
                            oa << v;
                        }

                        std::string buf { os.str() };
                        sock.send_to(boost::asio::buffer(buf), ep);

                    } catch (rashm::timeout_error const & e) {
                        BOOST_LOG_TRIVIAL(info) << "timeout (last "
                                << sr.lastReceptionTime() << "/"
                                << sr.headerTime() << ")";
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

template<typename DATAVEC>
struct SenderFactory {
    typedef SenderBase type;
    typedef std::shared_ptr<type> value_type;
    typedef NetConfig config_type;

    template<typename DATA, typename ID>
    static value_type make(config_type const& cfg) {
        return std::make_shared<Sender<DATAVEC, DATA, ID>>(cfg);
    }
};

template<typename DATAVEC, class VISITOR>
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
            typedef typename rashm::apply_all_data_ids<DATAVEC,
                    rashm::packet_functor_t>::type all_packets_t;

            typedef typename boost::make_variant_over<all_packets_t>::type all_packet_variant_t;

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

struct IdentityVisitor: public boost::static_visitor<void> {

    template<typename P>
    void operator()(P const& p) const;

    template<typename DATA, typename ID>
    void operator()(rashm::Packet<DATA, ID> const& p) const {
        BOOST_LOG_TRIVIAL(debug) << "received (a) " << p.name() << " "
                << p.head.timestamp;
        rashm::SegmentWriter<DATA, ID> sw; // Todo: Expensive construction
        sw = p.data;
    }

};

template<typename DATAVEC, typename VISITOR>
static void receive(NetConfig const& cfg) {
    boost::asio::io_service io;

    udp_server<DATAVEC, VISITOR> srv { cfg, io };

    boost::asio::deadline_timer timer { io, boost::posix_time::seconds(
            cfg.duration) };
    timer.async_wait([&io]( boost::system::error_code const& ) {io.stop();});

    io.run();
}

}

#endif /* SHMEM_RASHM_SHM_NET_H_ */

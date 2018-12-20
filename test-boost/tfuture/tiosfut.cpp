/*
 * tiosfut.cpp
 *
 *  Created on: 09.11.2018
 *      Author: netz
 */

#include <chrono>

#include <boost/asio.hpp>
#include <boost/signals2.hpp>

#include <boost/test/unit_test.hpp>

namespace {

using clock_t = std::chrono::steady_clock;

template<typename T>
class IosReceiver {
public:

    using msg_sig_t = typename T::msg_sig_t;
    using err_sig_t = typename T::err_sig_t;

    IosReceiver() :
            recv(), msg_signal(recv.msg_signal), err_signal(recv.err_signal) {
    }

    void setTimeout(clock_t::duration);

    T recv;
    msg_sig_t& msg_signal;
    err_sig_t& err_signal;

};

struct RecvMessage {

};

struct TestReceiver {
    using message_t = RecvMessage;

    enum eError {
        TIMEOUT, READ_ERR,
    };

    using msg_sig_t = boost::signals2::signal<void(const message_t&)>;
    using err_sig_t = boost::signals2::signal<void(eError)>;

    void configure();
    void handleReceivedData();
    void handleTimeout();

    msg_sig_t msg_signal;
    err_sig_t err_signal;
};

}

BOOST_AUTO_TEST_CASE( ios1 ) {
    IosReceiver<TestReceiver> recv;
    BOOST_CHECK(true);
}


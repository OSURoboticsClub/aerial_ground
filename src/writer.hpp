#ifndef WRITER_HPP_
#define WRITER_HPP_

#include "protocol/protocol.hpp"
#include "protocol/messages.hpp"

#include <boost/asio.hpp>
#include <mutex>

void writer(boost::asio::serial_port& port, protocol::message::offboard_attitude_control_message_t& msg, std::mutex& write_msg_mutex);

#endif

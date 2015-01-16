#ifndef READER_HPP_
#define READER_HPP_

#include <boost/asio.hpp>

#include "protocol/protocol.hpp"
#include "protocol/messages.hpp"
#include "protocol/decoder.hpp"

void reader(boost::asio::serial_port& port);

template <std::size_t buffer_size>
void handle(const protocol::decoded_message_t<buffer_size>& decoded);

#include "reader.tpp"

#endif

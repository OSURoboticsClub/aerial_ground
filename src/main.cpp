#include <iostream>

#include <boost/asio.hpp>

#include "protocol/protocol.hpp"
#include "protocol/messages.hpp"
#include "protocol/decoder.hpp"

template <std::size_t buffer_size>
void handle(const protocol::decoded_message_t<buffer_size>& decoded) {
  switch(decoded.id) {
    case protocol::message::heartbeat_message_t::ID: {
      auto message = *reinterpret_cast<const protocol::message::heartbeat_message_t *>(&decoded.payload);
      std::cout << "HEARTBEAT: " << (int) message.seq << std::endl;
      break;
    }
    case protocol::message::log_message_t::ID: {
      auto message = *reinterpret_cast<const protocol::message::log_message_t *>(&decoded.payload);
      std::cout << "LOG: " << message.data << std::endl;
      break;
    }
    case protocol::message::attitude_message_t::ID: {
      auto message = *reinterpret_cast<const protocol::message::attitude_message_t *>(&decoded.payload);
      std::cout << "ATTITUDE: " << message.roll << " " << message.pitch << " " << message.yaw << std::endl;
      break;
    }
    default:
      std::cout << "UNKNOWN: " << std::hex << decoded.id << std::dec << std::endl;
      break;
  }
}

int main(int argc, char **argv) {
  if(argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <tty>" << std::endl;
    return EXIT_FAILURE;
  }

  boost::asio::io_service io;
  boost::asio::serial_port port(io, argv[1]);

  port.set_option(boost::asio::serial_port_base::baud_rate(115200));
  port.set_option(boost::asio::serial_port::parity(boost::asio::serial_port::parity::none));
  port.set_option(boost::asio::serial_port::stop_bits(boost::asio::serial_port::stop_bits::one));
  port.set_option(boost::asio::serial_port::character_size(8));

  protocol::Decoder decoder;
  while(true) {
    char buffer[1];
    boost::asio::read(port, boost::asio::buffer(buffer));

    protocol::decoded_message_t<255> decoded;
    if(decoder.process(buffer[0], &decoded)) {
      std::cout << "Got message"
        << " with id=" << std::hex << (int) decoded.id << std::dec
        << " and size=" << (int) decoded.size << std::endl;
      handle(decoded);
    }
  }
}

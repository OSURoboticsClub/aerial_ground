#include "reader.hpp"

#include <chrono>
#include <iostream>

#include "protocol/protocol.hpp"
#include "protocol/messages.hpp"
#include "protocol/decoder.hpp"

void reader(boost::asio::serial_port& port) {
  protocol::Decoder decoder;

  while(true) {
    char buffer[1];
    boost::asio::read(port, boost::asio::buffer(buffer));

    protocol::decoded_message_t<255> decoded;
    if(decoder.process(buffer[0], &decoded)) {
      // std::cout << "Got message"
      //   << " with id=" << std::hex << (int) decoded.id << std::dec
      //   << " and size=" << (int) decoded.size << std::endl;
      handle(decoded);
    }
  }
}

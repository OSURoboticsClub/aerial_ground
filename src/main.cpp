#include <chrono>
#include <iostream>
#include <thread>
#include <mutex>

#include <boost/asio.hpp>

#include "protocol/protocol.hpp"
#include "protocol/messages.hpp"
#include "protocol/decoder.hpp"
#include "protocol/encoder.hpp"

#include "reader.hpp"
#include "writer.hpp"

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

  std::thread r(reader, std::ref(port));
  std::thread w(writer, std::ref(port));

  r.join();
  w.join();
}

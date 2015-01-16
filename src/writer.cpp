#include "writer.hpp"

#include <chrono>
#include <iostream>
#include <thread>

#include "protocol/protocol.hpp"
#include "protocol/messages.hpp"
#include "protocol/encoder.hpp"

void writer(boost::asio::serial_port& port) {
  protocol::Encoder encoder;

  std::uint8_t seq = 0;
  while(true) {
    std::array<std::uint8_t, 255> buffer;
    // protocol::message::heartbeat_message_t msg {
    //   .seq = seq++
    // };
    protocol::message::offboard_attitude_control_message_t msg {
      .roll = 10.0f,
      .pitch = 10.0f,
      .yaw = 10.0f,
      .throttle = 10.0f
    };
    std::uint16_t len = encoder.encode(msg, &buffer);

    boost::asio::write(port, boost::asio::buffer(buffer.data(), len));

    std::chrono::milliseconds dura(5);
    std::this_thread::sleep_for(dura);
  }
}

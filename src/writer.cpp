#include "writer.hpp"

#include <chrono>
#include <iostream>
#include <thread>

#include "protocol/protocol.hpp"
#include "protocol/messages.hpp"
#include "protocol/encoder.hpp"

static void sendArm(boost::asio::serial_port& port) {
  protocol::Encoder encoder;

    std::array<std::uint8_t, 255> buffer;
    protocol::message::set_arm_state_message_t msg {
      .armed = true
    };
    std::uint16_t len = encoder.encode(msg, &buffer);

    boost::asio::write(port, boost::asio::buffer(buffer.data(), len));
}

void writer(boost::asio::serial_port& port, protocol::message::offboard_attitude_control_message_t& msg, std::mutex& write_msg_mutex) {
  protocol::Encoder encoder;

  std::array<std::uint8_t, 255> buffer;

  // Send arm message
  protocol::message::set_arm_state_message_t armMsg {
    .armed = true
  };
  std::uint16_t len = encoder.encode(armMsg, &buffer);
  boost::asio::write(port, boost::asio::buffer(buffer.data(), len));

  std::uint8_t seq = 0;
  while(true) {
    write_msg_mutex.lock();
    std::uint16_t len = encoder.encode(msg, &buffer);
    //printf("%f %f %f %f\n", msg.roll, msg.pitch, msg.yaw, msg.throttle);   // DEBUG
    write_msg_mutex.unlock();

    boost::asio::write(port, boost::asio::buffer(buffer.data(), len));

    std::chrono::milliseconds dura(10);
    std::this_thread::sleep_for(dura);
  }
}

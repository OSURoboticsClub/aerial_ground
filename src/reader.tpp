#include <iostream>
#include <iomanip>
#include "test.hpp"

template <std::size_t buffer_size>
void handle(const protocol::decoded_message_t<buffer_size>& decoded) {
  switch(decoded.id) {
    case protocol::message::heartbeat_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::heartbeat_message_t&>(decoded.payload);
      std::cout << "<heartbeat>: " << (int) message.seq << std::endl;
      break;
    }
    case protocol::message::log_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::log_message_t&>(decoded.payload);
      std::cout << "<log>: " << message.data << std::endl;
      break;
    }
    case protocol::message::attitude_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::attitude_message_t&>(decoded.payload);
      std::cout << "<attitude>: " << std::fixed << std::setprecision(2)
        << "" << message.dcm[0] << " " << message.dcm[1] << " " << message.dcm[2]
        << " | " << message.dcm[3] << " " << message.dcm[4] << " " << message.dcm[5]
        << " | " << message.dcm[6] << " " << message.dcm[7] << " " << message.dcm[8] << std::endl;

      // 1 0 0 -> 0 0 1
      // 0 1 0 -> 1 0 0
      // 0 0 1 -> 0 1 0
      // x y z -> z x y

      vertices[6] =  message.dcm[0] * 0.5;
      vertices[7] =  message.dcm[2] * 0.5;
      vertices[8] =  message.dcm[1] * 0.5;
      vertices[18] = message.dcm[3] * 0.5;
      vertices[19] = message.dcm[5] * 0.5;
      vertices[20] = message.dcm[4] * 0.5;
      vertices[30] = message.dcm[6] * 0.5;
      vertices[31] = message.dcm[8] * 0.5;
      vertices[32] = message.dcm[7] * 0.5;

      break;
    }
    case protocol::message::motor_throttle_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::motor_throttle_message_t&>(decoded.payload);
      std::cout << "<throttle>: ";
      for(int i = 0; i < 4; i++) {
        std::cout << std::fixed << std::setprecision(2) << message.throttles[i] << " ";
      }
      std::cout << std::endl;
      break;
    }
    default:
      std::cout << "<UNKNOWN>: " << std::hex << decoded.id << std::dec << std::endl;
      break;
  }
}


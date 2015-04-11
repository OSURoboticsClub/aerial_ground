#include <iostream>
#include <iomanip>
#include "vis.hpp"

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
      printf("<attitude>: % .2d -- % .6f % .6f % .6f | % .6f % .6f % .6f | % .6f % .6f % .6f\n",
          message.time,
          message.dcm[0],
          message.dcm[1],
          message.dcm[2],
          message.dcm[3],
          message.dcm[4],
          message.dcm[5],
          message.dcm[6],
          message.dcm[7],
          message.dcm[8]);

      // 1 0 0 -> 0 0 1
      // 0 1 0 -> 1 0 0
      // 0 0 1 -> 0 1 0
      // x y z -> z x y
      updateVis(message);
      break;
    }
    case protocol::message::motor_throttle_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::motor_throttle_message_t&>(decoded.payload);
      std::cout << "<throttle>: ";
      for(int i = 0; i < 4; i++) {
        std::cout << std::fixed << std::setprecision(2) << message.time << " -- ";
        std::cout << std::fixed << std::setprecision(2) << message.throttles[i] << " ";
      }
      std::cout << std::endl;
      break;
    }
    case protocol::message::location_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::location_message_t&>(decoded.payload);
      std::cout << "<location>: ";
      std::cout << std::fixed << std::setprecision(2) << message.time << " -- ";
      std::cout << std::fixed << std::setprecision(2) << message.lat << " ";
      std::cout << std::fixed << std::setprecision(2) << message.lon << " ";
      std::cout << std::fixed << std::setprecision(2) << message.alt << " ";
      std::cout << std::endl;
      break;
    }
    case protocol::message::imu_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::imu_message_t&>(decoded.payload);
      std::cout << "<imu>: ";
      std::cout << std::fixed << std::setprecision(6) << message.time << " -- ";
      std::cout << std::fixed << std::setprecision(6) << message.gyro[0] << " ";
      std::cout << std::fixed << std::setprecision(6) << message.gyro[1] << " ";
      std::cout << std::fixed << std::setprecision(6) << message.gyro[2] << " ";
      std::cout << std::fixed << std::setprecision(6) << message.accel[0] << " ";
      std::cout << std::fixed << std::setprecision(6) << message.accel[1] << " ";
      std::cout << std::fixed << std::setprecision(6) << message.accel[2] << " ";
      std::cout << std::endl;
      break;
    }
    case protocol::message::system_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::system_message_t&>(decoded.payload);
      std::cout << "<system>: ";
      std::cout << std::fixed << message.time << " -- ";
      //std::cout << std::fixed << message.state << " ";
      printf("%d ", message.state);
      std::cout << std::fixed << std::setprecision(6) << message.motorDC << " ";
      std::cout << std::endl;
      break;
    }
    default:
      std::cout << "<UNKNOWN>: " << std::hex << decoded.id << std::dec << std::endl;
      break;
  }
}


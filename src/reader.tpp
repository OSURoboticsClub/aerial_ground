#include <iostream>
#include <iomanip>
#include <sstream>
#include "vis.hpp"

template <std::size_t buffer_size>
void handle(const protocol::decoded_message_t<buffer_size>& decoded) {
  std::string type = "UNKNOWN";
  std::stringstream msgstr;
  //msgstr << std::hex << decoded.id << std::dec;

  switch(decoded.id) {
    case protocol::message::heartbeat_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::heartbeat_message_t&>(decoded.payload);
      type = "heartbeat";
      msgstr << (int) message.seq;
      break;
    }
    case protocol::message::log_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::log_message_t&>(decoded.payload);
      type = "log";
      msgstr << message.data;
      break;
    }
    case protocol::message::attitude_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::attitude_message_t&>(decoded.payload);
      type = "attitude";
      msgstr << message.time << " -- ";
      msgstr << std::fixed << std::setprecision(2);
      msgstr << message.dcm[0] << " ";
      msgstr << message.dcm[1] << " ";
      msgstr << message.dcm[2] << " ";
      msgstr << message.dcm[3] << " ";
      msgstr << message.dcm[4] << " ";
      msgstr << message.dcm[5] << " ";
      msgstr << message.dcm[6] << " ";
      msgstr << message.dcm[7] << " ";
      msgstr << message.dcm[8];

      // 1 0 0 -> 0 0 1
      // 0 1 0 -> 1 0 0
      // 0 0 1 -> 0 1 0
      // x y z -> z x y
      updateVis(message);
      break;
    }
    case protocol::message::motor_throttle_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::motor_throttle_message_t&>(decoded.payload);
      type = "throttle";
      msgstr << message.time << " -- ";
      msgstr << std::fixed << std::setprecision(2);
      for(int i = 0; i < 4; i++) {
        msgstr << message.throttles[i] << " ";
      }
      break;
    }
    case protocol::message::location_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::location_message_t&>(decoded.payload);
      type = "location";
      msgstr << message.time << " -- ";
      msgstr << std::fixed << std::setprecision(2);
      msgstr << message.lat << " ";
      msgstr << message.lon << " ";
      msgstr << message.alt;
      break;
    }
    case protocol::message::imu_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::imu_message_t&>(decoded.payload);
      type = "imu";
      msgstr << message.time << " -- ";
      msgstr << std::fixed << std::setprecision(6);
      msgstr << message.gyro[0] << " ";
      msgstr << message.gyro[1] << " ";
      msgstr << message.gyro[2] << " ";
      msgstr << message.accel[0] << " ";
      msgstr << message.accel[1] << " ";
      msgstr << message.accel[2];
      break;
    }
    case protocol::message::system_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::system_message_t&>(decoded.payload);
      type = "system";
      msgstr << message.time << " -- ";
      msgstr << (int) message.state << " ";
      msgstr << std::fixed << std::setprecision(6);
      msgstr << message.motorDC;
      break;
    }
    case protocol::message::fs_info_message_t::ID: {
      auto message = reinterpret_cast<const protocol::message::fs_info_message_t&>(decoded.payload);
      type = "filesystem";
      msgstr << message.time << " -- ";
      msgstr << message.fname << " ";
      msgstr << message.fsize;
      break;
    }
    default:
      msgstr << "ID: " << (int) decoded.id;
      break;
  }
  std::cout << "<" << std::setw(12) << type << ">: " << msgstr.str() << std::endl;
}


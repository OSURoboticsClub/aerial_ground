#include <iostream>
#include <iomanip>

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
      break;
    }
    default:
      std::cout << "<UNKNOWN>: " << std::hex << decoded.id << std::dec << std::endl;
      break;
  }
}


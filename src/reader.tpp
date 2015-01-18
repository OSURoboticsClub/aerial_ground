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
      printf("<attitude>: % .6f % .6f % .6f | % .6f % .6f % .6f | % .6f % .6f % .6f\n", 
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

      // Draw axes RGB to XYZ
      vertices[6*1+0] = message.dcm[0] * 0.5;
      vertices[6*1+1] = message.dcm[1] * 0.5;
      vertices[6*1+2] = message.dcm[2] * 0.5;
      vertices[6*3+0] = message.dcm[3] * 0.5;
      vertices[6*3+1] = message.dcm[4] * 0.5;
      vertices[6*3+2] = message.dcm[5] * 0.5;
      vertices[6*5+0] = message.dcm[6] * 0.5;
      vertices[6*5+1] = message.dcm[7] * 0.5;
      vertices[6*5+2] = message.dcm[8] * 0.5;

      // Calculate wirefrarme box coordinates
      float box[24];
      for (int i=0; i<3; i++) {
        box[0*3+i] = ( message.dcm[0+i] + message.dcm[3+i] + 0.5*message.dcm[6+i]) * 0.2;
        box[1*3+i] = (-message.dcm[0+i] + message.dcm[3+i] + 0.5*message.dcm[6+i]) * 0.2;
        box[2*3+i] = ( message.dcm[0+i] - message.dcm[3+i] + 0.5*message.dcm[6+i]) * 0.2;
        box[3*3+i] = (-message.dcm[0+i] - message.dcm[3+i] + 0.5*message.dcm[6+i]) * 0.2;
        box[4*3+i] = ( message.dcm[0+i] + message.dcm[3+i] - 0.5*message.dcm[6+i]) * 0.2;
        box[5*3+i] = (-message.dcm[0+i] + message.dcm[3+i] - 0.5*message.dcm[6+i]) * 0.2;
        box[6*3+i] = ( message.dcm[0+i] - message.dcm[3+i] - 0.5*message.dcm[6+i]) * 0.2;
        box[7*3+i] = (-message.dcm[0+i] - message.dcm[3+i] - 0.5*message.dcm[6+i]) * 0.2;
      }

      for (int i=0; i<3; i++) {
        // Lines from (1,1,1)
        vertices[6*6+i] = box[3*0+i];
        vertices[6*7+i] = box[3*1+i];
        vertices[6*8+i] = box[3*0+i];
        vertices[6*9+i] = box[3*2+i];
        vertices[6*10+i] = box[3*0+i];
        vertices[6*11+i] = box[3*4+i];

        // Lines from (-1,-1,1)
        vertices[6*12+i] = box[3*3+i];
        vertices[6*13+i] = box[3*1+i];
        vertices[6*14+i] = box[3*3+i];
        vertices[6*15+i] = box[3*2+i];
        vertices[6*16+i] = box[3*3+i];
        vertices[6*17+i] = box[3*7+i];

        // Lines from (-1,1,-1)
        vertices[6*18+i] = box[3*5+i];
        vertices[6*19+i] = box[3*1+i];
        vertices[6*20+i] = box[3*5+i];
        vertices[6*21+i] = box[3*4+i];
        vertices[6*22+i] = box[3*5+i];
        vertices[6*23+i] = box[3*7+i];

        // Lines from (1,-1,-1)
        vertices[6*24+i] = box[3*6+i];
        vertices[6*25+i] = box[3*2+i];
        vertices[6*26+i] = box[3*6+i];
        vertices[6*27+i] = box[3*4+i];
        vertices[6*28+i] = box[3*6+i];
        vertices[6*29+i] = box[3*7+i];
      }

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


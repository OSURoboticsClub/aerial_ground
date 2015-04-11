#include "vis.hpp"

void updateVis(protocol::message::attitude_message_t message)
{
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
}

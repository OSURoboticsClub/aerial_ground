#ifndef TEST_HPP_
#define TEST_HPP_

#include "protocol/messages.hpp"

extern float vertices[6 * 30];

void updateVis(protocol::message::attitude_message_t message);

#endif

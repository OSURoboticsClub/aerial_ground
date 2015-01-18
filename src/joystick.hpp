#ifndef JOYSTICK_HPP_
#define JOYSTICK_HPP_

#include <fcntl.h>
#include <linux/joystick.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include <thread>
#include <mutex>

#include "protocol/protocol.hpp"
#include "protocol/messages.hpp"

#define BUTTON_ZERO_THROTTLE       0
//#define BUTTON_UNDEFINED           1
#define BUTTON_INC_ANG_POS_CAP     2
//#define BUTTON_UNDEFINED           3
#define BUTTON_RESET_YAW           4
#define BUTTON_ANG_POS_P_GAIN      5
#define BUTTON_ANG_VEL_P_GAIN      6
#define BUTTON_ANG_VEL_D_GAIN      7
#define BUTTON_Z_MOD               8   // Modify PPD gains for Z axis instead of XY.
//#define BUTTON_UNDEFINED           9
//#define BUTTON_UNDEFINED           10
#define BUTTON_INCREMENT_GAIN      11
#define BUTTON_DECREMENT_GAIN      12
//#define BUTTON_UNDEFINED           13

static bool armed = true;   // TODO(yoos): make false until throttle zero

static int16_t axes[3];
static uint16_t throttle = 3;
static int mode = 1;   // Velocity TODO(yoos): Need to import from FC code.
static int bf = 0x7fff;

static int Kx = -1;   // Type of gain to tune. <-1, 0, 1, 2>: <None, PP, VP, VD>.   TODO(yoos): HACK
static int axis = 0;   // 0: XY   1: Z

void joystick(int js, protocol::message::offboard_attitude_control_message_t& msg, std::mutex& write_msg_mutex);

#endif // JOYSTICK_HPP_

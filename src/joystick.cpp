#include "joystick.hpp"

void joystick(int js, protocol::message::offboard_attitude_control_message_t& msg, std::mutex& write_msg_mutex) {
  float throttle_left = 0;
  float throttle_right = 0;

  struct js_event e;
  while (true) {
    //printf("joystick\n");
    read(js, &e, sizeof(e));

    if (e.type == JS_EVENT_AXIS) {
      std::lock_guard<std::mutex> lock(write_msg_mutex);
      switch (e.number) {
        case 0:
          msg.roll = e.value / 32767.;
          break;
        case 1:
          msg.pitch = -e.value / 32767.;
          break;
        case 2:
          throttle_left = (-e.value + 0x7fff) / 65534.;
          msg.throttle = 0.2 * throttle_left + 0.8 * throttle_right;

          if(!armed && throttle < 10) {
            armed = true;
            printf("Arming!\n");
          }
          break;
        case 3:
          msg.yaw = e.value / 32767.;
          break;
        case 4:
          throttle_right = (-e.value + 0x7fff) / 65534.;
          msg.throttle = 0.2 * throttle_left + 0.8 * throttle_right;
          break;
        default:
          break;
      }
    }
    else if (e.type == JS_EVENT_BUTTON) {
      std::lock_guard<std::mutex> lock(write_msg_mutex);
      msg.buttons = ((0 << e.number) & msg.buttons) + (e.value << e.number);
      switch (e.number) {
        case BUTTON_ZERO_THROTTLE:
          break;
        case BUTTON_INC_ANG_POS_CAP:
          break;
        case BUTTON_RESET_YAW:
          break;

          // Controller gain tuning
        case BUTTON_ANG_POS_P_GAIN:
          if (e.value) Kx = 0;
          else Kx = -1;
          break;
        case BUTTON_ANG_VEL_P_GAIN:
          if (e.value) Kx = 1;
          else Kx = -1;
          break;
        case BUTTON_ANG_VEL_D_GAIN:
          if (e.value) Kx = 2;
          else Kx = -1;
          break;
        case BUTTON_Z_MOD:
          if (e.value) axis = 1;
          else axis = 0;
          break;
        case BUTTON_INCREMENT_GAIN:
          break;
        case BUTTON_DECREMENT_GAIN:
          break;

        default:
          break;
      }
    }

    std::chrono::milliseconds dura(1);
    std::this_thread::sleep_for(dura);
  }
}

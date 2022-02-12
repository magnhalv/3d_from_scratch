#ifndef INPUT_H
#define INPUT_H

#include "types.h"

typedef struct {
  i32 half_transition_count;
  bool ended_down;
} GameButtonState;

typedef struct {
  /* bool is_analog;
  bool is_connected;

  f32 stick_average_x;
  f32 stick_average_y; */

  f32 cursor_dx;
  f32 cursor_dy;

  union
  {
    GameButtonState buttons[14];
    struct {
      GameButtonState move_up;
      GameButtonState move_down;
      GameButtonState move_left;
      GameButtonState move_right;
      GameButtonState action_up;
      GameButtonState action_down;
      GameButtonState action_left;
      GameButtonState action_right;

      GameButtonState left_shoulder;
      GameButtonState right_shoulder;

      GameButtonState fire;

      GameButtonState back;
      GameButtonState start;

      //Note: All buttons must be added above this line
      GameButtonState terminator;
    };
  };
} GameControllerInput;

#endif
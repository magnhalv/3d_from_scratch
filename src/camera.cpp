#include "camera.h"

Camera camera = {
    .position = { 0.0, 10.0, -0.0 },
    .direction = { 0.0, 0.0, -1.0},
    .forward_velocity = { 0.0, 0.0, 0.0 },
    .yaw = 0,
    .pitch = M_PI/2
};


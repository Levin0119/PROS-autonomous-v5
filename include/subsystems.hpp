#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;
// Variables used for the claw control
constexpr double CLAW_CLOSED = 0.0;
constexpr double CLAW_OPEN = 67.0;
constexpr int CLAW_SPEED = 66.7;  // RPM
// Your motors, sensors, etc. should go here.  Below are examples

inline pros::Motor claw(15);
inline pros::MotorGroup lift({1, -2});
// inline pros::adi::DigitalIn limit_switch('A');
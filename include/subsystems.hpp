#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

// Your motors, sensors, etc. should go here.  Below are examples

inline pros::Motor claw(15);
inline pros::MotorGroup lift({1, -2});
// inline pros::adi::DigitalIn limit_switch('A');
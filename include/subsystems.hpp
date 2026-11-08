#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

// Your motors, sensors, etc. should go here.  Below are examples

inline pros::Motor intake(15);
inline pros::MotorGroup lift({2, -3});
// inline pros::adi::DigitalIn limit_switch('A');
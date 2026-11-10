#include "main.h"
#include "lift_control.hpp"
#include <cassert>
int main(){
using namespace lift_control;
initialize();begin_driver();
master.up=true;update(true);assert(lift.target==90);int n=lift.commands;
update(true);assert(lift.commands==n);
master.up=false;update(true);master.up=true;update(true);assert(lift.target==180);
master.up=false;master.down=true;update(true);assert(lift.target==90);
assert(move_to_level(3));assert(lift.target==270);
lift.positions={270,270};assert(wait_until_done());assert(status()==Status::reached);
assert(!move_to_level(4));assert(!move_to_motor_degrees(271));
assert(!move_to_lift_degrees(10));
assert(move_to_motor_degrees(120,25));lift.positions={120,120};assert(wait_until_done());
assert(move_to_level(0));pros::competition::disabled=true;assert(!wait_until_done());
assert(!move_to_level(1));pros::competition::disabled=false;
assert(move_to_level(1));stop();assert(!wait_until_done());
assert(move_to_level(2));pros::now=4001;assert(!wait_until_done());assert(status()==Status::fault);
assert(!move_to_level(0));
}

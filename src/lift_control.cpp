#include "main.h"
#include "lift_control.hpp"
#include <cmath>
#include <mutex>

namespace lift_control {
namespace {
pros::Mutex mutex;
using Guard = std::lock_guard<pros::Mutex>;
int level = 0;
Status state = Status::idle;
bool initialized = false;
bool previous_up = false;
bool previous_down = false;
unsigned started = 0;
unsigned generation = 0;
double target = 0;

void cancel_locked() {
  lift.brake();
  if (state != Status::fault) state = Status::cancelled;
  ++generation;
}
void fail_locked(const char* reason) {
  cancel_locked();
  state = Status::fault;
  master.print(0, 0, "Lift: %s", reason);
  master.rumble("---");
}
bool read_locked(double& a, double& b) {
  const auto positions = lift.get_position_all();
  if (positions.size() != 2 || !std::isfinite(positions[0]) ||
      !std::isfinite(positions[1])) {
    fail_locked("sensor error");
    return false;
  }
  a = positions[0];
  b = positions[1];
  if (a < min_motor_deg - limit_margin_deg ||
      b < min_motor_deg - limit_margin_deg ||
      a > max_motor_deg + limit_margin_deg ||
      b > max_motor_deg + limit_margin_deg) {
    fail_locked("travel limit");
    return false;
  }
  return true;
}
void service_locked() {
  if (!initialized || state == Status::fault) return;
  if (pros::competition::is_disabled()) {
    if (state == Status::moving) cancel_locked();
    return;
  }
  double a, b;
  if (!read_locked(a, b) || state != Status::moving) return;
  if (std::abs(a - target) <= tolerance_deg &&
      std::abs(b - target) <= tolerance_deg) {
    lift.brake();
    state = Status::reached;
  } else if (pros::millis() - started >= timeout_ms) {
    fail_locked("timeout");
  }
}
bool command_locked(double degrees, int rpm) {
  service_locked();
  if (!initialized || state == Status::fault ||
      pros::competition::is_disabled() || !std::isfinite(degrees) ||
      degrees < min_motor_deg || degrees > max_motor_deg || rpm <= 0 || rpm > 600) {
    cancel_locked();
    return false;
  }
  // Preserve the deadline across mid-motion replacements.
  if (state != Status::moving) started = pros::millis();
  target = degrees;
  ++generation;
  if (lift.move_absolute(target, rpm) == PROS_ERR) {
    fail_locked("motor error");
    return false;
  }
  state = Status::moving;
  // Arbitrary autonomous targets map to the nearest preset for driver handoff.
  level = 0;
  for (int i = 1; i <= 3; ++i)
    if (std::abs(heights[i] - target) < std::abs(heights[level] - target)) level = i;
  return true;
}
void monitor() {
  while (true) {
    { Guard lock(mutex); service_locked(); }
    pros::delay(10);
  }
}
}  // namespace

void initialize() {
  Guard lock(mutex);
  if (initialized) return;  // Never silently re-zero on a mode transition.
  if (lift.set_encoder_units_all(pros::E_MOTOR_ENCODER_DEGREES) == PROS_ERR ||
      lift.set_brake_mode_all(pros::E_MOTOR_BRAKE_HOLD) == PROS_ERR ||
      lift.tare_position_all() == PROS_ERR) {
    fail_locked("init error");
    return;
  }
  initialized = true;
  lift.brake();
  static pros::Task monitor_task(monitor);
}
void stop() { Guard lock(mutex); cancel_locked(); }
void begin_driver() {
  stop();
  previous_up = master.get_digital(DIGITAL_UP);
  previous_down = master.get_digital(DIGITAL_DOWN);
}
void update(bool controls_available) {
  const bool up = master.get_digital(DIGITAL_UP);
  const bool down = master.get_digital(DIGITAL_DOWN);
  const bool up_press = up && !previous_up;
  const bool down_press = down && !previous_down;
  previous_up = up;
  previous_down = down;
  Guard lock(mutex);
  service_locked();
  if (state == Status::fault) return;
  if (!controls_available || (up && down)) {
    if (state == Status::moving) cancel_locked();
    return;
  }
  int next = level;
  if (up_press && next < 3) ++next;
  if (down_press && next > 0) --next;
  if (next != level && command_locked(heights[next], speed_rpm))
    master.print(0, 0, "Lift level: %d   ", level);
}
bool move_to_level(int requested, int rpm) {
  Guard lock(mutex);
  if (requested < 0 || requested > 3) { cancel_locked(); return false; }
  return command_locked(heights[requested], rpm);
}
bool move_to_motor_degrees(double degrees, int rpm) {
  Guard lock(mutex);
  return command_locked(degrees, rpm);
}
bool move_to_lift_degrees(double degrees, int rpm) {
  Guard lock(mutex);
  if (motor_turns_per_lift_turn <= 0) { cancel_locked(); return false; }
  return command_locked(degrees * motor_turns_per_lift_turn, rpm);
}
bool wait_until_done() {
  unsigned command;
  { Guard lock(mutex); command = generation; }
  while (true) {
    {
      Guard lock(mutex);
      service_locked();
      if (generation != command) return false;
      if (state != Status::moving) return state == Status::reached;
    }
    pros::delay(10);
  }
}
bool move_to_level_and_wait(int requested, int rpm) {
  return move_to_level(requested, rpm) && wait_until_done();
}
Status status() { Guard lock(mutex); service_locked(); return state; }
double motor_position() {
  Guard lock(mutex);
  double a, b;
  if (!initialized || !read_locked(a, b)) return NAN;
  return (a + b) / 2.0;
}
}  // namespace lift_control

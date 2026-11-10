#pragma once

namespace lift_control {
// MOTOR encoder degrees, not vertical height. Calibrate these on the DR4B.
constexpr double heights[] = {0.0, 90.0, 180.0, 270.0};
constexpr double min_motor_deg = 0.0;
constexpr double max_motor_deg = 270.0;  // Calibrate separately from presets.
// Motor output-shaft turns per driven lift-shaft turn (external reduction).
// Example: 12T driving 60T -> 5.0. Zero means unknown; shaft API is disabled.
constexpr double motor_turns_per_lift_turn = 0.0;
constexpr int speed_rpm = 30;
constexpr double tolerance_deg = 5.0;
constexpr double limit_margin_deg = 10.0;
constexpr unsigned timeout_ms = 4000;

enum class Status { idle, moving, reached, cancelled, fault };
void initialize();  // Once at boot, physically at bottom. Starts monitor task.
void stop();        // Cancel without re-zeroing.
void begin_driver();
void update(bool controls_available);  // Driver buttons only.

// Nonblocking commands. False means rejected: do not proceed with scoring.
// Positions are absolute relative to startup zero. Speed is motor-shaft RPM.
// A command rejected while moving cancels that movement.
bool move_to_level(int level, int rpm = speed_rpm);
bool move_to_motor_degrees(double degrees, int rpm = speed_rpm);
bool move_to_lift_degrees(double degrees, int rpm = speed_rpm);
// Wait for the current command. True only when it reaches its target;
// cancellation, replacement, disabled mode, and faults return false.
bool wait_until_done();
bool move_to_level_and_wait(int level, int rpm = speed_rpm);
Status status();
double motor_position();  // Average of both encoders, NAN if unavailable.
}  // namespace lift_control

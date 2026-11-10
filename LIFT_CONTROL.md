# DR4B lift control

- Start the program with the lift physically at the bottom. Encoders are zeroed
  only at startup, not when changing competition modes.
- Up selects the next target level; Down selects the previous target level.
  Holding a button does not repeat. Target changes are accepted during movement.
- Levels 0/1/2/3 default to 0/90/180/270 motor encoder degrees at 30 RPM.
  These are commissioning placeholders, not measured heights or safe travel.
  Edit `include/lift_control.hpp` after testing the actual mechanism.
- Both arrows together cancel movement and hold the current position. The
  selected level remains unchanged; the next press steps from that selection.
- HOLD is configured for both lift motors. Disabled competition control cannot
  provide powered holding; support the mechanism as needed.
- The software checks both encoders against 0..270 degrees with a 10-degree
  overshoot margin. A movement has a 4-second deadline, including mid-movement
  target changes. Sensor, command, travel and timeout faults cancel movement and
  latch until restart. Inspect/support the lift and return it to the bottom
  before restarting. HOLD remains active after a fault; this is not power-off.
- PID tuning owns the arrow buttons while enabled. B+Down retains the template's
  autonomous shortcut. These modes cancel lift movement.

There is no physical limit switch or automatic homing. Software cannot detect an
incorrect startup zero or guarantee protection against mechanical hard stops.
First test unloaded with space to move and a way to disable the robot. Confirm
both motors lift in the correct direction, then calibrate each height and the
maximum travel before normal use. DR4B height is not necessarily proportional to
motor rotation. Confirm that the 4-second deadline suits the final travel.

## Autonomous API

`main.h` exposes the lift API to `autons.cpp`. A background PROS task checks both
encoders every 10 ms in driver and autonomous modes, including while chassis
calls block. Do not command the raw `lift` MotorGroup elsewhere: use this API so
its target, deadline and fault state stay consistent. Only one caller should own
lift commands at a time. Startup zero persists across autonomous/driver changes.

Sequential movement inside an autonomous routine:

```cpp
if (!lift_control::move_to_level_and_wait(2)) return;
// Perform the scoring action here only after success.
if (!lift_control::move_to_level_and_wait(0)) return;
```

Move while driving (insert into a routine deliberately):

```cpp
if (!lift_control::move_to_level(1)) return;
chassis.pid_drive_set(12_in, 60);
chassis.pid_wait();
if (!lift_control::wait_until_done()) return;
```

Fine adjustment without changing the driver presets:

```cpp
if (!lift_control::move_to_motor_degrees(150.0, 30)) return;
if (!lift_control::wait_until_done()) return;
```

Commands return false for invalid arguments, disabled mode or a latched fault;
a rejected command cancels any previous movement. Waiting returns false for
faults, cancellation or command replacement. `status()` reports progress and
`motor_position()` reads average motor degrees (NAN on a sensor/range error).
A wait concerns the most recent command; use one command owner at a time.

## Calibration

All settings live in `include/lift_control.hpp`:

- `heights`: bottom plus three preset MOTOR angles.
- `min_motor_deg` / `max_motor_deg`: independently calibrated software limits.
  Updating the top preset does not automatically increase the travel limit.
- `speed_rpm`, `tolerance_deg`, `timeout_ms`: motion tuning.
- `motor_turns_per_lift_turn`: external gear reduction, motor output-shaft turns
  per driven lift-shaft turn. 12T driving 60T means 5.0; multiply each stage for
  compound gearing. This excludes the motor cartridge's internal reduction.
  Default 0 disables `move_to_lift_degrees()` until the ratio is known.

After setting that ratio, `move_to_lift_degrees(angle, rpm)` converts lift drive
shaft rotation relative to the startup pose into motor encoder degrees. It is
NOT a vertical-height command. Existing presets and software limits stay in
motor degrees, and rpm always refers to the motor output shaft.

To implement a vertical-height API for a DR4B, supply linkage geometry, gearing,
and the bottom pose, or a measured table of motor angles versus platform heights.
A calibrated table allows interpolation within measured travel; do not assume
constant height per motor revolution for a linkage. No height conversion is
silently enabled with invented geometry.

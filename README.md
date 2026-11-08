# VEX V5 robot code

This PROS C++ project controls a four-motor VEX V5 drivetrain, a claw, and a two-motor lift. It uses EZ-Template for driver control, PID tuning, and autonomous routines.

## Current status

Updated: 2026-11-08.

The robot has split arcade drive, manual claw control, and manual lift control. The team's autonomous routines currently move the chassis only; claw and lift actions have not yet been integrated into those routines. Pure Pursuit and Boomerang are included as template examples.

Lift control currently uses direct motor power commands. There are no programmed height presets, travel limits, or automatic lift-position control. Hardware direction and operation still need to be checked on the robot; this README describes the implementation, not a new hardware test.

## Driver controls

| Input | Action |
| --- | --- |
| Sticks | Drive and steer using split arcade control |
| Hold R1 | Drive the claw motor at +30 |
| Hold R2 | Drive the claw motor at -30 |
| Release R1 and R2 | Set claw motor power to 0 |
| Hold D-pad Up | Raise the lift: motor command +127 |
| Hold D-pad Down | Lower the lift: motor command -127 |
| Release Up and Down | Set lift motor power to 0 |

Claw opening/closing direction depends on the mechanism and motor installation; confirm which button opens or closes the claw on the robot. Releasing a button sets motor power to zero; the current code does not explicitly configure a lift hold brake mode.

For code navigation: the claw motor is still named `intake` in the source. It refers to the claw on port 15, not an intake mechanism.

## Working on the project

Open the folder containing [project.pros](project.pros) in VS Code. With the PROS toolchain installed, run `pros make` from that folder to build the program. The project uses PROS kernel 4.1.1 and EZ-Template 3.2.2.

- [src/main.cpp](src/main.cpp): driver controls, drivetrain and sensor ports, and the list of routines available on the Brain.
- [src/autons.cpp](src/autons.cpp): PID settings and autonomous routines. The team's routines are `my_forward`, `my_leftGoal`, `my_rightGoal`, and `my_test`.
- [include/subsystems.hpp](include/subsystems.hpp): claw motor and lift motor-group definitions.

## Configured V5 ports

| Device | Smart port(s) |
| --- | --- |
| Left drive | 13 and 14 (both reversed) |
| Right drive | 11 and 12 |
| Claw | 15 |
| Lift | 2 and 3 (port 3 reversed) |
| IMU | 18 |
| Vertical / horizontal tracking wheels | 19 (reversed) / 20 |

## Backups

The [2026-11-02 snapshot notes](backups/VEX-V5-2026-11-02-no-lift/README.md) describe the version with chassis and claw control, before lift implementation. The [archived project ZIP](backups/VEX-V5-2026-11-02-no-lift/VEX-V5-2026-11-02-no-lift.zip) preserves that version.

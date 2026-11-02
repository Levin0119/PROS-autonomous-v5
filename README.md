# VEX V5 robot code

This PROS C++ project controls a four-motor VEX V5 drivetrain and an intake. It uses EZ-Template for driver control, PID tuning, and autonomous routines.

## Current status

The robot has split arcade drive and manual intake control. **Lift control is not implemented yet.** The team's autonomous routines currently move the chassis only. Pure Pursuit and Boomerang are included as template examples.

## Driver controls

| Input | Action |
| --- | --- |
| Sticks | Drive and steer using split arcade control |
| Hold R1 | Run the intake forward |
| Hold R2 | Run the intake in reverse |
| Release both buttons | Stop the intake |

## Working on the project

Open the folder containing [project.pros](project.pros) in VS Code. With the PROS toolchain installed, run `pros make` from that folder to build the program. The project uses PROS kernel 4.1.1 and EZ-Template 3.2.2.

- [src/main.cpp](src/main.cpp): driver controls, drivetrain and sensor ports, and the list of routines available on the Brain.
- [src/autons.cpp](src/autons.cpp): PID settings and autonomous routines. The team's routines are `my_forward`, `my_leftGoal`, `my_rightGoal`, and `my_test`.
- [include/subsystems.hpp](include/subsystems.hpp): intake motor definition.

## Configured V5 ports

| Device | Smart port(s) |
| --- | --- |
| Left drive | 13 and 14 (both reversed) |
| Right drive | 11 and 12 |
| Intake | 15 |
| IMU | 18 |
| Vertical / horizontal tracking wheels | 19 (reversed) / 20 |

## Backups

The [2026-11-02 snapshot notes](backups/VEX-V5-2026-11-02-no-lift/README.md) describe the version with chassis and intake control, before lift implementation. The [archived project ZIP](backups/VEX-V5-2026-11-02-no-lift/VEX-V5-2026-11-02-no-lift.zip) preserves that version.

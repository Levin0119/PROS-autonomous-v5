#pragma once
#include <vector>
#include <cstdint>
#define PROS_ERR 2147483647
#define DIGITAL_UP 1
#define DIGITAL_DOWN 2
namespace pros {
enum {E_MOTOR_ENCODER_DEGREES, E_MOTOR_BRAKE_HOLD};
inline unsigned now=0;
inline unsigned millis(){return now;}
}
struct Motor {
std::vector<double> positions{0,0};
int commands=0, brakes=0; double target=0;
int set_encoder_units_all(int){return 1;}
int set_brake_mode_all(int){return 1;}
int tare_position_all(){positions={0,0};return 1;}
int brake(){++brakes;return 1;}
auto get_position_all(){return positions;}
int move_absolute(double p,int){target=p;++commands;return 1;}
};
struct Controller {
bool up=false,down=false;
bool get_digital(int button){return button==DIGITAL_UP?up:down;}
void print(int,int,const char*,...){}
void rumble(const char*){}
};
inline Motor lift;
inline Controller master;
#include <mutex>
namespace pros {
using Mutex = std::mutex;
struct Task { template<class F> Task(F){} };
namespace competition { inline bool disabled=false; inline bool is_disabled(){return disabled;} }
}
namespace pros { inline void delay(unsigned ms){now+=ms;} }

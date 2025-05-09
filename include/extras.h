#ifndef EXTRAS_H
#define EXTRAS_H



// Extra functions
float DeadBand(float input, float width);
float ToVolt(float percent);
float ToMM(float degrees);

template <class T>
vex::task launch_task(T&& function);

namespace vex
{
    template<typename T>
    T clamp(T val, T mn, T mx){
        return std::max(std::min(val, mx), mn);
    }
}

extern const float LoadPosition;
extern const float HoverPosition;
extern float TrackerWheelSize;
extern bool Loading;




int conveyorback();
int lbdown();
int qlbrown();
extern float MogoTime;
int mogo();
int mogo2();
int conveyor();
int intakeconveyor();
int ringstop2();

int mogodrop();
int lbload();
int lbloadconveyor();
extern bool AJEnable;
int mogowait();
int ringstop();
extern float locktime;
int colock();
int jamdetect();
int lbstop();
int cstop();
extern float waitTime;
int ldoinker();
int rdoinker();
int unjam();
int corner();
int ringstop3();
extern float ConveyorTime;

#endif
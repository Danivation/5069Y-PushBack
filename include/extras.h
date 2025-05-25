#pragma once



// Extra functions
float DeadBand(float input, float width);
float ToVolt(float percent);
float ToMM(float degrees);

template <class T>
vex::task launch_task(T&& function);

// vex::clamp
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

#pragma once



// Extra functions
float DeadBand(float input, float width);
float ToVolt(float percent);
float ToMM(float degrees);
float toDegrees(float angle);
float toRadians(float angle);
float reduce_radians(float angle);
float reduce_0_to_360(float angle);
float trueHeading();
float trueRotation();


// vex::clamp and vex::launch_task
namespace vex
{
    template <class F>
    vex::task launch_task(F&& function) {
        //static_assert(std::is_invocable_r_v<void, F>);
        return vex::task([](void* parameters) {
            std::unique_ptr<std::function<void()>> ptr{static_cast<std::function<void()>*>(parameters)};
            (*ptr)();
            return 0;
        }, new std::function<void()>(std::forward<F>(function)));
    }
    
    template<typename T>
    T clamp(T val, T mn, T mx){
        return std::max(std::min(val, mx), mn);
    }
}

extern const float LoadPosition;
extern const float HoverPosition;
extern float TrackerWheelSize;
extern bool Loading;

#pragma once
#include "vex.h"

class odometry
{
public:
    class tracker
    {
    public:
        tracker(vex::rotation *sensor, float offset, float wheelSize);
        float getPosition();
        float getOffset();

    private:
        vex::rotation *p_sensor;
        float p_offset;
        float p_wheelSize;
    };
    odometry(odometry::tracker *t_horizontal, odometry::tracker *t_vertical);
    void startTracking(float x, float y, float theta);
    void stopTracking();
    void setPose(float x, float y, float theta);
    VecPose currentPose = {0, 0, 0};
    VecPose getPose();

private:
    bool odomEnabled = true;
    odometry::tracker *pt_horizontal;
    odometry::tracker *pt_vertical;
};
extern odometry Odometry;




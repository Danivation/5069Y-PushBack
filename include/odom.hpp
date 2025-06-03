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





class odom {
private:
    bool odomEnabled = true;

    float getVerticalChange(float& previousVerticalPosition);
    float getHorizontalChange(float& previousHorizontalPosition);
    float getChangeInRotation(float& previousRotation);
    void runOdom(float initialX, float initialY, float initialHeading);

    float ForwardTracker_center_distance = 0;
    float SidewaysTracker_center_distance = 0;
    float ForwardTracker_position = 0;
    float SideWaysTracker_position = 0;

    float prevForward = 0;
    float prevSideways = 0;
    float prevTheta = 0;

public:
    // Current pose used elsewhere
    VecPose currentOdomPose = {0, 0, 0};


    float X_position = 0;
    float Y_position = 0;
    odom();

    void startOdom(float startX, float startY, float startHeading);
    void setPose(float x, float y, float heading);
    //VecPose getPose();
    //void follow(CubicBezier curve, bool forwards, float timeout, bool splitCurve);
    void update_position(float ForwardTracker_position, float SidewaysTracker_position, float orientation_deg);

};
extern odom Odom;

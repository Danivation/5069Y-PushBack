using namespace vex;
using signature = vision::signature;
using code = vision::code;

extern brain Brain;

extern controller Controller;
extern inertial Inertial1;
extern inertial Inertial2;
extern rotation HTracker;
extern rotation VTracker;

extern motor ConveyorMotor;
extern motor IntakeMotor;
extern motor LBMotorL;
extern motor LBMotorR;
extern motor_group LBMotors;

extern motor LA;
extern motor LB;
extern motor LC;
extern motor RA;
extern motor RB;
extern motor RC;

extern motor_group L;
extern motor_group R;

extern digital_out Clamp;
extern digital_out DoinkerL;
extern digital_out DoinkerR;

extern digital_in LBLimit;

extern rotation LBRotation;

extern distance RingDistance;
extern optical RingColor;

extern signature OldFrontVision__RING_RED;
extern signature OldFrontVision__RING_BLUE;
extern signature OldFrontVision__MOGO_YELLOW;
extern vision OldFrontVision;

extern aivision FrontVision;

//extern signature BackVision__RING_RED;
//extern signature BackVision__RING_BLUE;
//extern signature BackVision__MOGO_YELLOW;
//extern vision BackVision;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */

void vexcodeInit(void);

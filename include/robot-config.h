using namespace vex;
using signature = vision::signature;
using code = vision::code;

extern brain Brain;

extern controller Controller;
extern inertial Inertial1;
extern inertial Inertial2;
extern rotation HTracker;
extern rotation VTracker;

extern motor LA;
extern motor LB;
extern motor LC;
extern motor RA;
extern motor RB;
extern motor RC;

extern motor_group L;
extern motor_group R;

extern motor IntakeL;
extern motor IntakeR;
extern motor_group Intake;

extern digital_out Lift;
extern digital_out Trapdoor;
extern digital_out Loader;
extern digital_out Stopper;
extern digital_out Parker;

extern optical BlockColor;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
**/

void vexcodeInit(void);

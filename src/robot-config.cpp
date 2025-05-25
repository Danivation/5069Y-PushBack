#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// VEXcode device constructors
brain Brain;
controller Controller   (primary);

motor LA                (PORT11, ratio6_1, true);
motor LB                (PORT12, ratio6_1, true);
motor LC                (PORT13, ratio6_1, true);
motor RA                (PORT14, ratio6_1, false);
motor RB                (PORT15, ratio6_1, false);
motor RC                (PORT16, ratio6_1, false);

motor_group L           (LA, LB, LC);
motor_group R           (RA, RB, RC);

rotation HTracker       (PORT1, false);    // CORRECT
rotation VTracker       (PORT2, false);    // CORRECT
inertial Inertial1      (PORT3);
inertial Inertial2      (PORT4);


// VEXcode generated functions
// define variable for remote controller enable/disable
bool RemoteControlCodeEnabled = true;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void) {
  // nothing to initialize
}
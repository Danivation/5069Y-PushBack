/*----------------------------------------------------------------------------*/
/*                                                                            */
/*    Module:       vex.h                                                     */
/*    Author:       Vex Robotics                                              */
/*    Created:      1 Feb 2019                                                */
/*    Description:  Default header for V5 projects                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/
//
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include <memory>
#include <limits>
#include <utility>
#include <stdexcept>
#include <algorithm>

#include "v5.h"
#include "v5_vcs.h"

#include "robot-config.h"

#include "controller.h"
#include "pid.h"
#include "extras.h"
#include "colorsort.h"
#include "auton.h"
#include "odom.hpp"
#include "ramsete.hpp"
#include "pypaths.hpp"


#include "CubicBezier.hpp"
#include "Math/Pose.hpp"
#include "Math/MathFunctions.hpp"

#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
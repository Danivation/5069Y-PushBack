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

#include "controller.hpp"
#include "pid.hpp"
#include "extras.hpp"
#include "colorsort.hpp"
#include "auton.hpp"
#include "odom.hpp"
#include "oldramsete.hpp"
#include "pypaths.hpp"

#include "CubicBezier.hpp"
#include "MathFunctions.hpp"

#include "vmplib/types.hpp"
#include "vmplib/motion-profiler.hpp"
#include "vmplib/motion-profiling.hpp"


#define waitUntil(condition)                                                   \
  do {                                                                         \
    wait(5, msec);                                                             \
  } while (!(condition))

#define repeat(iterations)                                                     \
  for (int iterator = 0; iterator < iterations; iterator++)
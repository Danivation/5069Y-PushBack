#include "vex.h"
using namespace vex;

bool ColorLock = false;

/*
 * COLOR SORT
 * ring passes by color sensor, if it detects a "near" object for a long time (cant be a hook)
 * of a red or blue color for a majority of the time,
 * then activate distance, start getting results
 * as soon as the next thing gets within 50 mm, start tracking conveyor rotations
 * after a certain number, flip if bad
**/

/**
// a pointer, containing a const vex::color, called SortClr, is pointing to the address of vex::transparent
const vex::color* SortColor = &transparent;
int ccount = 0;   // wrong color counter

bool Flipping = false; // if the ring is flipping, this is true
int ColorSort()
{
  RingColor.integrationTime(2);
  int tcount = 0;   // detection threshold counter
  //int rcount = 0;   // total ring counter
  int ncount = 0;   // no ring counter
  
  while (true)
  {
    if (RingColor.isNearObject())
    {
      tcount += 1;

      if (RingColor.color() == *SortColor)
      {
        ccount += 1;
      }
      
      if (tcount >= 9)    // if its been near an object for more than 9 cycles
      {
        //rcount += 1;
        // at this point, it knows there is a ring and how many there have been so far
        std::cout << "ring detected (color) with strength " << ccount << " after " << ncount << "\n";
        Flipping = true;
        task ds1(DistanceCheck);
        ncount = 0;
        tcount = 0;
        waitUntil(!RingColor.isNearObject());
        waitUntil(!Flipping);
        ccount = 0;
      }
      wait(3, msec);
    }
    else
    {
      tcount = 0;
      ncount += 1;
      wait(1, msec);
      ccount = 0;
    }
  }
}

float StartPosition = 0;
int strengthcounter = 0;
int DistanceCheck()
{
  strengthcounter = ccount;
  int tcount = 0;   // detection threshold counter
  //int rcount = 0;   // total ring counter
  int ncount = 0;   // no ring counter
  while (true)
  {
    if (RingDistance.objectDistance(mm) <= 45)
    {
      StartPosition = ConveyorMotor.position(turns);
      tcount += 1;
      //std::cout << "g \n";
      
      if (ncount >= 30)     // if its near smth for more than 3 cycles, and before that there was nothing for more than 30 cycles
      {
        std::cout << "ring detected (distance) after " << ncount << " with strength " << strengthcounter << "\n";
        ncount = 0;
        tcount = 0;
        if (strengthcounter >= 2)       // if its a 2 or over, flip it, if its a 0-1, dont
        {
          task rf1(RingFlip);
        }
        else
        {
          std::cout << "not flipping ring with strength " << strengthcounter << "\n";
          Flipping = false;
        }
        return 1;
        waitUntil(!(RingDistance.objectDistance(mm) <= 45));
      }
      wait(6, msec);
    }
    else
    {
      tcount = 0;
      ncount += 1;
      //std::cout << ncount << " nothing \n";
      wait(2, msec);
    }
  }
}

bool AJBuffer;
int RingFlip()
{
  int strcounter = strengthcounter;
  //StartPosition = ConveyorMotor.position(turns);
  waitUntil(ConveyorMotor.position(turns) >= StartPosition + 0.5);
  if (fabs(ConveyorMotor.velocity(rpm)) > 50 && ColorLock == false)
  {
    //ConveyorMotor.stop(hold);
    ConveyorMotor.spin(reverse, ToVolt(100), volt);
    std::cout << "flipping ring with strength " << strcounter << "\n";
    wait(100, msec);
    ConveyorMotor.spin(forward, ToVolt(100), volt);
    Flipping = false;
  }
  return 1;
}
/**/
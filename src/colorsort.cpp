#include "vex.h"
using namespace vex;

bool ColorLock = false;


/*
 * PUSH BACK COLOR SORT
 * block will pass by color sensor
 * when it does activate trapdoor after a certain amt of time and for a certain duration (no need for distance sensor, use color sensor's distance)
**/


const vex::color* SortColor = &transparent;
int ccount = 0;   // wrong color counter
int ColorSort()
{
  BlockColor.integrationTime(2);
  int tcount = 0;   // detection threshold counter
  //int rcount = 0;   // total block counter
  int ncount = 0;   // no block counter
  
  while (true)
  {
    if (BlockColor.isNearObject())
    {
      tcount += 1;

      if (BlockColor.color() == *SortColor)
      {
        ccount += 1;
      }
      
      if (tcount >= 9)    // if its been near an object for more than 9 cycles
      {
        std::cout << "block detected (color) with strength " << ccount << " after " << ncount << "\n";
        ncount = 0;
        tcount = 0;
        task release1(BlockRelease);
        // REPLACE THIS with something that says when the block has passed, maybe a redundant check for a new color for more than a few cycles
        //waitUntil(!BlockColor.isNearObject());
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

int BlockRelease()
{
  wait(200, msec);
  //Trapdoor.set(false);
  return 1;
}

/**
// a pointer, containing a const vex::color, called SortColor, is pointing to the address of vex::transparent
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
#ifndef COLORSORT_H
#define COLORSORT_H

#include "vex.h"

extern const vex::color* SortColor;
extern bool ColorLock;

int ColorSort();
int DistanceCheck();
int RingFlip();

#endif
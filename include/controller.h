#pragma once


// Controller functions
extern int selectedAuton;
extern int autonMode;
extern std::string autonName;
extern std::string autonModeName;
extern bool Bypass;


int AutonSelect();
int DrivetrainControl();
int ConveyorControl();
int ClampToggleControl();
int DoinkerLControl();
int DoinkerRControl();
int LBReset();
int LBControl();
int LBLoadControl();
int LBDescoreControl();
int LBDescore2Control();

int load();
int descore();
int descore2();

#pragma once


// Controller functions
extern int selectedAuton;
extern int autonMode;
extern std::string autonName;
extern std::string autonModeName;
extern bool Bypass;


int AutonSelect();
int DrivetrainControl();
int IntakeControl();
int LiftControl();
int TrapdoorControl();
int LoaderControl();
int StopperControl();
int ParkerControl();
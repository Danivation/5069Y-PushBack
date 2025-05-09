#ifndef PID_H
#define PID_H

// Drive control functions

extern bool Settled;

extern timer TimerDrive;
extern timer TimerTurn;
extern timer DriveTime;
extern timer TurnTime;

extern bool PidOn;     // enables pid globally
extern bool PidLock;  // locks pid, when finished
extern float Dist;            // error from setpoint
extern float Goal;            // setpoint
extern float DistInt;         // integral
extern float PDist;           // previous error
extern float Power;           // final speed output
extern float AutoAngle;
extern float CorrectionError;
extern float Magnitude;
extern float LeftOffset;      // left motor scaling
extern float RightOffset;     // right motor scaling
extern float LeftCorrection;
extern float RightCorrection;
extern bool CorrectLock;

extern bool Linear;           // whether driving straight or turning

extern float StartInt;   // integral start (integral limit)

extern float kp;
extern float ki;
extern float kd;

extern bool AlignLock;

extern int AColor;
extern float CenterDist;
extern float akp;
extern float SizeRatio;
extern float MinWidth;

extern float APower;

int TurnSettle();
int DriveSettle();
int PID();



void SwerveL(float Heading, float Timeout = 1, float KKP = 2.7, float KKI = 0.131, float KKD = 16.1, float SL = 1, float ST = 20);
void SwerveR(float Heading, float Timeout = 1, float KKP = 2.7, float KKI = 0.131, float KKD = 16.1, float SL = 1, float ST = 20);
void Turn(float Heading, float Timeout = 1, float KKP = 2.05, float KKI = 0.131, float KKD = 16.1, float SL = 1, float ST = 20);
void TurnS(float Heading, float Timeout, float SL = 1, float ST = 20, float KKP = 2.05);

void Drive(float Distance, float Timeout = 1, float KKP = 0.118, float KKI = 0, float KKD = -1, float SL = 5, float ST = 20);
void DriveS(float Distance, float Timeout, float SL = 5, float ST = 30, float KKP = 0.116, float KKI = 0, float KKD = -1, float DNSB = 0);

void Arc(float Distance, float Timeout, float LP, float RP, float KKP, float KKI, float KKD);

int Align();
void FrontAlign(int clr, float Timeout, float KP, float mwidth);


#endif
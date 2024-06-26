#include <Arduino.h>

enum MachineStatus {STOP, STARTING, RUNNING, STOPPING, STARTPOSITIONING, POSITIONING};
enum NeedleStopPositions {UP, DOWN, DontCare};
enum PositioningStatus {psPOSITIONING_STOPPED, psPOSITIONING_START, psPOSITIONING_RUNNING, psPOSITIONING_DONE};

extern const String MachineStatusText[];
extern const String NeedleStopPositionsText[];
extern MachineStatus StatusMachine;
extern NeedleStopPositions NeedleStopPostion;
extern PositioningStatus StatusHalfStitch;
extern PositioningStatus StatusOneStitch;


void STM_MachineStatus();
void MotorOff();
void MotorOn();
void ToogleNeedleStopPosition();

void STM_HalfStitch();
void StartHalfStitch();

void STM_OneSitch();
void StartOneStitch();
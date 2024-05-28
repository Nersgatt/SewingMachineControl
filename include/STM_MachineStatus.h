#include <Arduino.h>

enum MachineStatus {STOP, STARTING, RUNNING, STOPPING, STARTPOSITIONING, POSITIONING};
enum NeedleStopPositions {UP, DOWN, DontCare};

extern const String MachineStatusText[];
extern const String NeedleStopPositionsText[];
extern MachineStatus StatusMachine;
extern NeedleStopPositions NeedleStopPostion;

void STM_MachineStatus();
void MotorOff();
void MotorOn();
void ToogleNeedleStopPosition();
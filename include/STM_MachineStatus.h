#include <Arduino.h>

enum MachineStatus {STOP, STARTING, RUNNING, STOPPING};
enum NeedleStopPositions {UP, DOWN, DontCare};

extern const String MachineStatusText[4];
extern const String NeedleStopPositionsText[3];
extern MachineStatus StatusMachine;
extern NeedleStopPositions NeedleStopPostion;

void STM_MachineStatus();
void MotorOff();
void MotorOn();
void ToogleNeedleStopPosition();
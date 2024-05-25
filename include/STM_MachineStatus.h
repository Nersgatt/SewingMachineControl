#include <Arduino.h>

enum MachineStatus {STOP, STARTING, RUNNING, STOPPING};
enum NeedleStopPositions {UP, DOWN, DontCare};

extern const String MachineStatusText[4];
extern MachineStatus StatusMachine;

void STM_MachineStatus();
void MotorOff();
void MotorOn();

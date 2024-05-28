#include <Arduino.h>
#include <globals.h>
#include <STM_MachineStatus.h>
#include <STM_NeedleStatus.h>
#include <config.h>

const String MachineStatusText[4] = {"STOP", "STARTING", "RUNNING", "STOPPING"};
const String NeedleStopPositionsText[3] = {"Oben", "Unten", "Egal"};
MachineStatus StatusMachine = STOP;
NeedleStopPositions NeedleStopPostion = DontCare;

void STM_MachineStatus() {

  static int last_target_speed = 0;

  switch (StatusMachine)
  {
  case STOP:
    if (target_speed > 0) {
      StatusMachine = STARTING;
    }
    break;

  case STARTING:
    MotorOn();
    StatusMachine = RUNNING;
    break;

  case RUNNING:    
    if (last_target_speed != target_speed) {
      last_target_speed = target_speed;
      stepper.spin(target_speed * -1);    
    }

    if (target_speed == 0) {
      StatusMachine = STOPPING;
    }
    break;

  case STOPPING:
    if (target_speed > 0) {
      StatusMachine = RUNNING;
    } else {
      if ((stepper.speed() * -1) <= POSITIONING_SPEED) {

        if (((NeedleStopPostion == DOWN) && !IsNeedleDown()) ||
            ((NeedleStopPostion == UP) && !IsNeedleUp())) {
          // Geschwindigkeit halten, bis gewünschte Position gerreicht ist
          stepper.spin(POSITIONING_SPEED * -1); 
        } else {
          // Endposition erreicht
          stepper.spin(0);
          MotorOff();
          StatusMachine = STOP;
        }
      }
    }
    break;
  }

}

void MotorOff() {
  if (stepper.isPowered()) {
    stepper.powerOff();
  }
}

void MotorOn() {
  if (!stepper.isPowered()) {
    stepper.powerOn();
  }
}

void ToogleNeedleStopPosition() {
  switch (NeedleStopPostion) {
    case UP:
      NeedleStopPostion = DOWN;
      break;
    case DOWN:
      NeedleStopPostion = DontCare;
      break;
    case DontCare:
      NeedleStopPostion = UP;
      break;
  }
}
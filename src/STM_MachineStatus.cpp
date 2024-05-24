#include <Arduino.h>
#include <globals.h>
#include <STM_MachineStatus.h>

const String MachineStatusText[4] = {"STOP", "STARTING", "RUNNING", "STOPPING"};
MachineStatus StatusMachine = STOP;

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
      if (!stepper.isSpinning()) {
        MotorOff();
        StatusMachine = STOP;
      }
    }
    break;

  }

}

void MotorOff() {
  if (stepper.isPowerOn()) {
    stepper.powerOff();
  }
}

void MotorOn() {
  if (!stepper.isPowerOn()) {
    stepper.powerOn();
  }
}
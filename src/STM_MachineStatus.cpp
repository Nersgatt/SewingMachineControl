#include <Arduino.h>
#include <globals.h>
#include <STM_MachineStatus.h>
#include <STM_NeedleStatus.h>
#include <config.h>

const String MachineStatusText[] = {"STOP", "STARTING", "RUNNING", "STOPPING"};
const String NeedleStopPositionsText[] = {"Oben", "Unten", "Egal"};
MachineStatus StatusMachine = STOP;
NeedleStopPositions NeedleStopPostion = DontCare;
PositioningStatus StatusHalfStitch = psPOSITIONING_STOPPED;
PositioningStatus StatusOneStitch = psPOSITIONING_STOPPED;

void STM_MachineStatus() {

  static int last_target_speed = 0;
  static long t_PosStart = 0;

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
        StatusMachine = STARTPOSITIONING;
      }
    }    
    break;
  case STARTPOSITIONING:
    t_PosStart = CurrentMillis;
    StatusMachine = POSITIONING;
    break;
  case POSITIONING:
    if (target_speed > 0) {
      StatusMachine = RUNNING;
    } else {

      if (CurrentMillis - t_PosStart > MAX_POSITIONING_DELAY) {
        // Die Positionierung dauert zu lange. In den Fehlerzustand gehen
        stepper.spin(0);
        MotorOff();
        StatusMachine = STOP;
        ErrorText = "Positionierung?!";
        status = msENTER_ERROR;
      } else {

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

void STM_HalfStitch() {

  static NeedleStatus TargetStatus = nsOT_Triggered;
  static long t_PosStart = 0;

  switch (StatusHalfStitch)
  {
  case psPOSITIONING_STOPPED:
    break;
  case psPOSITIONING_START:
    t_PosStart = CurrentMillis;
    if (LastNeedlePostion == npOT) {
      TargetStatus = nsUT_Triggered;
    } else {
      TargetStatus = nsOT_Triggered;
    }

    stepper.spin(POSITIONING_SPEED * -1);
    MotorOn();
    StatusHalfStitch = psPOSITIONING_RUNNING;
    break;

  case psPOSITIONING_RUNNING:
    if (CurrentMillis - t_PosStart > MAX_POSITIONING_DELAY) {
      // Die Positionierung dauert zu lange. In den Fehlerzustand gehen
      stepper.spin(0);
      MotorOff();
      StatusHalfStitch = psPOSITIONING_STOPPED;
      ErrorText = "Positionierung?!";
      status = msENTER_ERROR;
    } else {

      if ((TargetStatus == nsOT_Triggered && IsNeedleUp()) ||
          (TargetStatus == nsUT_Triggered && StatusNeedle == nsUT_Triggered)) {
        stepper.spin(0); 
        MotorOff();
        StatusHalfStitch = psPOSITIONING_DONE;
      }
    }
    break;
  case psPOSITIONING_DONE:
    StatusHalfStitch = psPOSITIONING_STOPPED;
    break;  
  }
}

void STM_OneSitch() {

  static NeedleStatus TargetStatus = nsOT_Triggered;
  static long t_PosStart = 0;
  static NeedlePosition _np;
  static bool OneStitchDone;

  switch (StatusOneStitch)
  {
  case psPOSITIONING_STOPPED:
    break;
  case psPOSITIONING_START:
    _np = LastNeedlePostion;
    OneStitchDone = false;
    t_PosStart = CurrentMillis;
    if (_np == npOT) {
      TargetStatus = nsOT_Triggered;
    } else {
      TargetStatus = nsUT_Triggered;
    }

    stepper.spin(POSITIONING_SPEED * -1);
    MotorOn();
    StatusOneStitch = psPOSITIONING_RUNNING;
    break;

  case psPOSITIONING_RUNNING:
    if (LastNeedlePostion != _np) {OneStitchDone = true;}

    if (CurrentMillis - t_PosStart > (MAX_POSITIONING_DELAY * 2)) {
      // Die Positionierung dauert zu lange. In den Fehlerzustand gehen
      stepper.spin(0);
      MotorOff();
      StatusOneStitch = psPOSITIONING_STOPPED;
      ErrorText = "Positionierung?!";
      status = msENTER_ERROR;
    } else {

      if (OneStitchDone) {
        if ((TargetStatus == nsOT_Triggered && IsNeedleUp()) ||
            (TargetStatus == nsUT_Triggered && StatusNeedle == nsUT_Triggered)) {
          stepper.spin(0); 
          MotorOff();
          StatusOneStitch = psPOSITIONING_DONE;
        }
      }
    }
    break;
  case psPOSITIONING_DONE:
    StatusOneStitch = psPOSITIONING_STOPPED;
    break;  
  }
}

void StartHalfStitch() {
  StatusHalfStitch = psPOSITIONING_START;
}

void StartOneStitch() {
  StatusOneStitch = psPOSITIONING_START;
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
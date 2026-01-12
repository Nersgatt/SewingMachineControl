#include <Arduino.h>
#include <globals.h>
#include <STM_MachineStatus.h>
#include <STM_NeedleStatus.h>
#include <config.h>

const String MachineStatusText[] = {"STOP", "STARTING", "RUNNING", "STOPPING"};
const String NeedleStopPositionsText[] = {"Oben", "Unten", "Egal"};
MachineStatus StatusMachine = msSTOP;
NeedleStopPositions NeedleStopPostion = nspDontCare;
PositioningStatus StatusHalfStitch = psPOSITIONING_STOPPED;
PositioningStatus StatusOneStitch = psPOSITIONING_STOPPED;

void STM_MachineStatus() {

  static int last_target_speed = 0;
  static long t_PosStart = 0;

  switch (StatusMachine)
  {
  case msSTOP:
    if (target_speed > 0) {
      StatusMachine = msSTARTING;
    }
    break;

  case msSTARTING:
    MotorOn();
    StatusMachine = msRUNNING;
    break;

  case msRUNNING:    
    if (last_target_speed != target_speed) {
      last_target_speed = target_speed;
      stepper.spin(target_speed * -1);    
    }

    if (target_speed == 0) {
      StatusMachine = msSTOPPING;
    }
    break;

  case msSTOPPING:
    if (target_speed > 0) {
      StatusMachine = msRUNNING;
    } else {
      if ((stepper.speed() * -1) <= POSITIONING_SPEED) {
        StatusMachine = msSTARTPOSITIONING;
      }
    }    
    break;
  case msSTARTPOSITIONING:
    t_PosStart = CurrentMillis;
    StatusMachine = msPOSITIONING;
    break;
  case msPOSITIONING:
    if (target_speed > 0) {
      StatusMachine = msRUNNING;
    } else {

      if (NeedleStopPostion == nspDontCare) {
        // keine Positionierung gewünscht, Motor sofort anhalten
          stepper.spin(0);
          MotorOff();
          StatusMachine = msSTOP;
      } else if (CurrentMillis - t_PosStart > MAX_POSITIONING_DELAY) {
        // Die Positionierung dauert zu lange. In den Fehlerzustand gehen
        stepper.spin(0);
        MotorOff();
        StatusMachine = msSTOP;
        ErrorText = "Positionierung?!";
        status = msENTER_ERROR;
      } else {

        if (((NeedleStopPostion == nspDOWN) && !IsNeedleDown()) ||
            ((NeedleStopPostion == nspUP) && !IsNeedleUp())) {
          // Geschwindigkeit halten, bis gewünschte Position gerreicht ist
          stepper.spin(POSITIONING_SPEED * -1); 
        } else {
          // Endposition erreicht
          stepper.spin(0);
          MotorOff();
          StatusMachine = msSTOP;
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
      #ifdef DEBUG_INFO
      Serial.println("Start positioning, target UT");
      #endif
      TargetStatus = nsUT_Triggered;
    } else {
      #ifdef DEBUG_INFO
      Serial.println("Start positioning, target OT");
      #endif
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
    case nspUP:
      NeedleStopPostion = nspDOWN;
      break;
    case nspDOWN:
      NeedleStopPostion = nspDontCare;
      break;
    case nspDontCare:
      NeedleStopPostion = nspUP;
      break;
  }
}
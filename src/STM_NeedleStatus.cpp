#include <Arduino.h>
#include <STM_NeedleStatus.h>
#include <config.h>
#include <globals.h>

const String NeedleStatusText[] = {
    "UNKNOWN_Armed", "UNKNOWN_Wait", "UNKNOWN triggered", "Unknown hold", 
    "OT_Armed", "OT_Wait", "OT triggered", "OT_HOLD_Armed", "OT_HOLD_Wait", "OT hold", 
    "UT_Armed", "UT_Wait", "UT triggered", "UT hold"};

const String LastNeedlePosText[] {
  "OT", "UT"
};

NeedleStatus StatusNeedle = nsUNKNOWN_Hold;
#ifdef DEBUG_INFO
  NeedleStatus OldStatusNeedle;
#endif
NeedlePosition LastNeedlePostion = npOT;

void STM_NeedleStatus() {
  static long t0 = 0;

  #ifdef DEBUG_INFO
  OldStatusNeedle = StatusNeedle;
  #endif

  bool SensorOT = (digitalRead(PIN_OT) == HIGH);
  bool SensorUT = (digitalRead(PIN_UT) == HIGH);

  switch (StatusNeedle) {
    case nsUNKNOWN_Armed:
      if (SensorOT) {
        StatusNeedle = nsOT_Armed;
      } else if (SensorUT) {
        StatusNeedle = nsUT_Armed;

      } else {
        t0 = CurrentMillis;
        StatusNeedle = nsUNKNOWN_WAIT;
      }
      break;
    case nsUNKNOWN_WAIT:
      if (SensorOT) {
        StatusNeedle = nsOT_Armed;
      } else if (SensorUT) {
        StatusNeedle = nsUT_Armed;

      } else {
        if (CurrentMillis - t0 > NEEDLE_SENSOR_BOUNCE_DELAY) {
          StatusNeedle = nsUNKNOWN_Triggered;
        }
      }
      break;
    case nsUNKNOWN_Triggered:
      
      if (SensorOT) {
        StatusNeedle = nsOT_Armed;
      } else if (SensorUT) {
        StatusNeedle = nsUT_Armed;
      } else {
        StatusNeedle = nsUNKNOWN_Hold;
      }

      break;
    case nsUNKNOWN_Hold:

      if (SensorOT) {
        StatusNeedle = nsOT_Armed;
      } else if (SensorUT) {
        StatusNeedle = nsUT_Armed;
      }

      break;
    case nsOT_Armed:
      if (SensorOT) {
        t0 = CurrentMillis;
        StatusNeedle = nsOT_WAIT;
      } else if (SensorUT) {
        StatusNeedle = nsUT_Armed;
      } else {
        StatusNeedle = nsUNKNOWN_Armed;
      }
      break;
    case nsOT_WAIT:
      if (SensorOT) {
        if (CurrentMillis - t0 > NEEDLE_SENSOR_BOUNCE_DELAY) {
          StatusNeedle = nsOT_Triggered;
        }
      } else if (SensorUT) {
        StatusNeedle = nsUT_Armed;
      } else {
        StatusNeedle = nsUNKNOWN_Armed;
      }
      break;
    case nsOT_Triggered:
      LastNeedlePostion = npOT;
      if (SensorOT) {
        StatusNeedle = nsOT_HOLD_Armed;
      } else if (SensorUT) {
        StatusNeedle = nsUT_Armed;
      } else {
        StatusNeedle = nsUNKNOWN_Armed;
      }

      break;
    case nsOT_HOLD_Armed:
      t0 = CurrentMillis;
      StatusNeedle = nsOT_HOLD_Wait;
      break;
    case nsOT_HOLD_Wait:
      if (CurrentMillis - t0 > NEELDE_OT_HOLD_DELAY) {
        StatusNeedle = nsOT_Hold;
      }
      break;
    case nsOT_Hold:
      if (SensorUT) {
        StatusNeedle = nsUT_Armed;
      } else if (!SensorOT) {
        StatusNeedle = nsUNKNOWN_Armed;
      }

      break;
    case nsUT_Armed:
      if (SensorOT) {
        StatusNeedle = nsOT_Armed;
      } else if (SensorUT) {
        t0 = CurrentMillis;
        StatusNeedle = nsUT_WAIT;
      } else {
        StatusNeedle = nsUNKNOWN_Armed;
      }
      break;
    case nsUT_WAIT:
      if (SensorOT) {
        StatusNeedle = nsOT_Armed;
      } else if (SensorUT) {
        if (CurrentMillis - t0 > NEEDLE_SENSOR_BOUNCE_DELAY) {
          StatusNeedle = nsUT_Triggered;
        }
      } else {
        StatusNeedle = nsUNKNOWN_Armed;
      }
      break;
    case nsUT_Triggered:
      LastNeedlePostion = npUT;
      if (SensorUT) {
        StatusNeedle = nsUT_Hold;
      } else if (SensorOT) {
        StatusNeedle = nsOT_Armed;
      } else {
        StatusNeedle = nsUNKNOWN_Armed;
      }
      break;
    case nsUT_Hold:
      if (SensorOT) {
        StatusNeedle = nsOT_Armed;
      } else if (!SensorUT) {
        StatusNeedle = nsUNKNOWN_Armed;
      }
      break;
  }

  #ifdef DEBUG_INFO
  if (OldStatusNeedle != StatusNeedle) {
    Serial.print("Needle Status changed, Old: ");
    Serial.print(NeedleStatusText[OldStatusNeedle]);
    Serial.print(", New ");
    Serial.println(NeedleStatusText[StatusNeedle]);

    Serial.print("SensorOT: ");
    Serial.print(SensorOT);

    Serial.print(" SensorUT: ");
    Serial.println(SensorUT);

    Serial.print("LastNeedlePos: ");
    Serial.println(LastNeedlePosText[LastNeedlePostion]);
  }
  #endif
  

}

bool IsNeedleUp() {
  // return StatusNeedle == nsOT_Triggered || StatusNeedle == nsOT_Hold;
  return StatusNeedle == nsOT_Hold;
  // return LastNeedlePostion == npOT && StatusNeedle == nsUNKNOWN_Triggered;
};

bool IsNeedleDown() {
  return StatusNeedle == nsUT_Triggered || StatusNeedle == nsUT_Hold;
};
#include <Arduino.h>
#include <STM_NeedleStatus.h>
#include <config.h>

const String NeedleStatusText[6] = {"UNKNOWN triggered", "Unknown hold", "OT triggered", "OT hold", "UT triggered", "UT hold"};
NeedleStatus StatusNeedle = UNKNOWN_Hold;

void STM_NeedleStatus() {
  bool SensorOT = (digitalRead(PIN_OT) == LOW);
  bool SensorUT = (digitalRead(PIN_UT) == LOW);

  switch (StatusNeedle) {
    case UNKNOWN_Triggered:
      
      if (SensorOT) {
        StatusNeedle = OT_Triggered;
      } else if (SensorUT) {
        StatusNeedle = UT_Triggered;
      } else {
        StatusNeedle = UNKNOWN_Hold;
      }

      break;
    case UNKNOWN_Hold:

      if (SensorOT) {
        StatusNeedle = OT_Triggered;
      } else if (SensorUT) {
        StatusNeedle = UT_Triggered;
      }

      break;
    case OT_Triggered:

      if (SensorOT) {
        StatusNeedle = OT_Hold;
      } else if (SensorUT) {
        StatusNeedle = UT_Triggered;
      } else {
        StatusNeedle = UNKNOWN_Triggered;
      }

      break;
    case OT_Hold:
      
      if (SensorUT) {
        StatusNeedle = UT_Triggered;
      } else if (!SensorOT) {
        StatusNeedle = UNKNOWN_Triggered;
      }

      break;
    case UT_Triggered:

      if (SensorUT) {
        StatusNeedle = UT_Hold;
      } else if (SensorOT) {
        StatusNeedle = OT_Triggered;
      } else {
        StatusNeedle = UNKNOWN_Triggered;
      }
      break;
    case UT_Hold:
      if (SensorOT) {
        StatusNeedle = OT_Triggered;
      } else if (!SensorUT) {
        StatusNeedle = UNKNOWN_Triggered;
      }
      break;
  }

}

bool IsNeedleUp() {
  return StatusNeedle == OT_Triggered || StatusNeedle == OT_Hold;
};

bool IsNeedleDown() {
  return StatusNeedle == UT_Triggered || StatusNeedle == UT_Hold;
};
#include <Arduino.h>
enum ButtonStatus {RESET, WAIT, ARMING, ARMED, DRAWN, TRIGGERED, HOLD, LOWWAIT};
void STM_BTN_NeedlePosition();

extern ButtonStatus Status_BTN_NeedlePosition;
const String ButtonStatusText [9] = {"RESET", "WAIT", "ARMING", "ARMED", "DRAWN", "TRIGGERED", "HOLD", "LOWWAIT"};
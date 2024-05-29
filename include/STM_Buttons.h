#include <Arduino.h>
enum ButtonStatus {bsRESET, bsWAIT, bsARMING, bsARMED, bsDRAWN, bsTRIGGERED, bsHOLD, bsLOWWAIT};

extern ButtonStatus Status_BTN_NeedlePosition;
extern ButtonStatus Status_BTN_MoveNeedle;
extern ButtonStatus Status_BTN_OneStitch;

const String ButtonStatusText [9] = {"RESET", "WAIT", "ARMING", "ARMED", "DRAWN", "TRIGGERED", "HOLD", "LOWWAIT"};

void STM_BTN_NeedlePosition();
void STM_BTN_MoveNeedle();
void STM_BTN_OneStitch();
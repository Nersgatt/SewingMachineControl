#include <Arduino.h>

enum NeedleStatus {UNKNOWN_Triggered, UNKNOWN_Hold, OT_Triggered, OT_Hold, UT_Triggered, UT_Hold};

extern const String NeedleStatusText[6];
extern NeedleStatus StatusNeedle;

void STM_NeedleStatus();
bool IsNeedleUp();
bool IsNeedleDown();
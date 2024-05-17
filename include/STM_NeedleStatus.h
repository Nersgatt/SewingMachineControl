#include <Arduino.h>

enum NeedleStatus {
        nsUNKNOWN_Armed, nsUNKNOWN_WAIT, nsUNKNOWN_Triggered, nsUNKNOWN_Hold, 
        nsOT_Armed, nsOT_WAIT, nsOT_Triggered, nsOT_Hold, 
        nsUT_Armed, nsUT_WAIT, nsUT_Triggered, nsUT_Hold};
enum NeedlePosition {npOT, npUT};

extern const String NeedleStatusText[];
extern NeedleStatus StatusNeedle;
extern NeedlePosition LastNeedlePostion;

void STM_NeedleStatus();
bool IsNeedleUp();
bool IsNeedleDown();
#include <globals.h>
#include <STM_NeedleStatus.h>
#include <display.h>

void UpdateStichCount() {
  static unsigned long StichCnt = 0;
  if (StatusNeedle == nsOT_Triggered) {
    // Anzahl Stiche zählen und anzeigen
    StichCnt++;
    UpdateDisplayStichCount(StichCnt);
  }
}

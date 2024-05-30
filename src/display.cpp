#include <Arduino.h>
#include <globals.h>
#include <STM_MachineStatus.h>

byte customCharUp[] = {
  B00100,
  B01110,
  B10101,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

byte customCharDown[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B10101,
  B01110,
  B00100
};

byte customCharDontCare[] = {
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B00000,
  B00000,
  B00000
};

void CreateCustonCharacters() {
 lcd.createChar(0, customCharUp);    
 lcd.createChar(1, customCharDown);    
 lcd.createChar(2, customCharDontCare);    
}

void UpdateDisplayNeedlePosition() {
    lcd.setCursor(7, 1);
    switch (NeedleStopPostion)
    {
    case nspUP:
        lcd.write(0);
        break;
    case nspDOWN:
        lcd.write(1);
        break;
    case nspDontCare:
        lcd.write(2);
        break;
    }
}

void UpdateDisplayStichCount(unsigned int StichCount) {
  lcd.setCursor(8,0);
  lcd.print(StichCount);
}

void UpdateDisplayError() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("!! FEHLER !!");
  lcd.setCursor(0, 1);
  lcd.print(ErrorText);
  
}

void SetupDisplay() {
    lcd.clear();
    CreateCustonCharacters();

    lcd.setCursor(0, 0);
    lcd.print("Stiche: ");
    lcd.setCursor(0, 1);
    lcd.print("Nadel: ");   
}
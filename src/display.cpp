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

void UpdateDisplay() {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Nadel: ");
    switch (NeedleStopPostion)
    {
    case UP:
        lcd.write(0);
        break;
    case DOWN:
        lcd.write(1);
        break;
    case DontCare:
        lcd.write(2);
        break;
    }
}
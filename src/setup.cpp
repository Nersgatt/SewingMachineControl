#include <Arduino.h>
#include <config.h>
#include <globals.h>

void setupPins()
{
  pinMode(PIN_UT, INPUT);
  pinMode(PIN_OT, INPUT);

  pinMode(PIN_NEEDLE_UP, INPUT_PULLUP);
  pinMode(PIN_NEEDLE_DOWN, INPUT_PULLUP);
  pinMode(PIN_TOOGLE_POSITION, INPUT_PULLUP);
}

void setupStopValue() {  
  int val = analogRead(PIN_POT);
  HAL_SENSOR_STOP_VALUE = val + 10;
}

void setupStepper() {
  stepper.begin(PIN_PUL, PIN_DIR);
  stepper.setAcceleration(ACC);
  stepper.setEnablePin(PIN_ENA);
}

void setupTimer1() {
  noInterrupts();
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 10 Hz (16000000/((6249+1)*256))
  OCR1A = 6249;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 256
  TCCR1B |= (1 << CS12);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();  
}

void setupLCD() {
  lcd.init();
  lcd.backlight();
}
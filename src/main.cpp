#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <config.h>
#include <globals.h>
#include <setup.h>
#include <STM_NeedleStatus.h>
#include <STM_MachineStatus.h>
#include <STM_Buttons.h>
#include <display.h>

void setup() {

  #ifdef DEBUG_INFO
  Serial.begin(9600);
  #endif

  setupPins();
  setupStepper();
  setupStopValue();
  MotorOff();  
  setupTimer1();
  setupLCD();

  lcd.print("Pfaff 130");
  UpdateDisplay();
}


void loop() {

  // static int lastStatus = 0;

  CurrentMillis = millis();

  STM_NeedleStatus();
  STM_MachineStatus();
  if (StatusMachine != STOP) {
    stepper.loop();
  } else {
    STM_BTN_NeedlePosition();

    #ifdef DEBUG_INFO
    // if (state_s1 != lastStatus) {
    //   lastStatus = state_s1;      
    //   Serial.println(ButtonStatusText[state_s1]);      
    // }
    #endif  

    if (Status_BTN_NeedlePosition == TRIGGERED) {
      ToogleNeedleStopPosition();
      UpdateDisplay();
    }
  }


}



// Timer 1 interrupt service routine (ISR)
// Wertet die Positon des Fußpedals aus
ISR(TIMER1_COMPA_vect)
{
  int val = analogRead(PIN_POT);

  if (val < HAL_SENSOR_STOP_VALUE) {
    target_speed = 0;
  } else if (val > HAL_SENSOR_FULL_THROTTLE_VALUE) {
    target_speed = MAX_SPEED;    
  } else {
    target_speed = map(val, HAL_SENSOR_FULL_THROTTLE_VALUE, HAL_SENSOR_STOP_VALUE, MAX_SPEED, 0);
  }
  
}


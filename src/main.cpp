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

void UpdateStichCount() {
  static unsigned long StichCnt = 0;
  if (StatusNeedle == nsOT_Triggered) {
    // Anzahl Stiche zählen und anzeigen
    StichCnt++;
    UpdateDisplayStichCount(StichCnt);
  }
}

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

  UpdateDisplayStichCount(0);
  UpdateDisplayNeedlePosition();
}


void loop() {

  CurrentMillis = millis();

  switch (status) {
    case msSEWING:
      STM_NeedleStatus();
      STM_MachineStatus();

      if (StatusMachine != STOP) {
        stepper.loop();
        UpdateStichCount();
      } else {
        // Maschine reagiert nur auf Buttons, wenn der Fußanlasser nicht betätigt wird
        STM_BTN_NeedlePosition();
        STM_BTN_MoveNeedle();
        STM_BTN_OneStitch();

        if (Status_BTN_NeedlePosition == TRIGGERED) {
          ToogleNeedleStopPosition();
          UpdateDisplayNeedlePosition();
        }

        if (Status_BTN_MoveNeedle == TRIGGERED) {
          status = msPOSITIONING;
        }

        if (Status_BTN_OneStitch == TRIGGERED) {
          status = msOneStitch;
        }
      }
      break;
    case msPOSITIONING:      
      if (StatusPositioning == psPOSITIONING_STOPPED) {
        StartPositioning();
      }
      STM_NeedleStatus();
      STM_Positioning();
      if (StatusPositioning == psPOSITIONING_DONE) {
        status = msSEWING;
      }
      stepper.loop();
      UpdateStichCount();
      break;
    case msOneStitch:      
      if (StatusOneStitch == psPOSITIONING_STOPPED) {
        StartOneStitch();
      }
      // Serial.print("LastNeedlePos "); Serial.print(LastNeedlePostion); Serial.println("");
      STM_NeedleStatus();

      STM_OneSitch();
      if (StatusOneStitch == psPOSITIONING_DONE) {
        status = msSEWING;
      }
      stepper.loop();
      UpdateStichCount();
      break;
    case msENTER_ERROR:
      UpdateDisplayError();      
      status = msERROR;
      break;
    case msERROR:
      break;
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


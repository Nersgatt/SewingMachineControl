#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

#include <config.h>
#include <globals.h>
#include <setup.h>
#include <STM_NeedleStatus.h>
#include <STM_MachineStatus.h>


void setup() {

  #ifdef DEBUG_INFO
  Serial.begin(9600);
  #endif

  setupPins();
  setupStepper();
  setupStopValue();
  MotorOff();  
  setupTimer1();
}


void loop() {
  // static NeedleStatus LastStatusNeedle = UNKNOWN_Hold;
  unsigned long CurrentMillis = millis();
  static unsigned long LastUTMillis = CurrentMillis;
  float SecsPerStitch;

  STM_NeedleStatus();
  STM_MachineStatus();
  stepper.loop();

  if (StatusNeedle == UT_Triggered) {
    SecsPerStitch = float(CurrentMillis - LastUTMillis) / 1000;
    LastUTMillis = CurrentMillis;

    #ifdef DEBUG_INFO
      Serial.print("Stitches/Sec: ");
      Serial.println(1/SecsPerStitch);
    #endif
  }

  #ifdef DEBUG_INFO
    // Serial.print("Status ");
    // Serial.println(StatusText[Status]);

    // if (LastStatusNeedle != StatusNeedle) {
    //   Serial.println(NeedleStatusText[StatusNeedle]);
    //   LastStatusNeedle = StatusNeedle;
    // }
  #endif  
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


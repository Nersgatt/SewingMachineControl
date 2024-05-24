#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

#include <config.h>
#include <globals.h>

int HAL_SENSOR_STOP_VALUE = 530;

int target_speed = 0;
int last_target_speed = 0;
boolean isPowerOff = false;

enum MachineStatus {STOP, STARTING, RUNNING, STOPPING};
const String StatusText[4] = {"STOP", "STARTING", "RUNNING", "STOPPING"};
MachineStatus Status = STOP;

enum NeedleStatus {UNKNOWN_Triggered, UNKNOWN_Hold, OT_Triggered, OT_Hold, UT_Triggered, UT_Hold};
const String NeedleStatusText[6] = {"UNKNOWN triggered", "Unknown hold", "OT triggered", "OT hold", "UT triggered", "UT hold"};
NeedleStatus StatusNeedle = UNKNOWN_Hold;

ContinuousStepper<StepperDriver, ToneTicker> stepper;

void setupStepper();
void setupStopValue();
void setupTimer1() ;

void STM_Main();
void STM_NeedleStatus();

void MotorOff();
void MotorOn();

void setup() {

  #ifdef DEBUG_INFO
  Serial.begin(9600);
  #endif

  pinMode(PIN_UT, INPUT);
  pinMode(PIN_OT, INPUT);
  
  setupStepper();
  setupStopValue();
  MotorOff();  
  setupTimer1();

}

void STM_Main() {

  switch (Status)
  {
  case STOP:
    if (target_speed > 0) {
      Status = STARTING;
    }
    break;

  case STARTING:
    MotorOn();
    Status = RUNNING;
    break;

  case RUNNING:    
    if (last_target_speed != target_speed) {
      last_target_speed = target_speed;
      stepper.spin(target_speed * -1);    
    }

    if (target_speed == 0) {
      Status = STOPPING;
    }
    break;

  case STOPPING:
    if (target_speed > 0) {
      Status = RUNNING;
    } else {
      if (!stepper.isSpinning()) {
        MotorOff();
        Status = STOP;
      }
    }
    break;

  }

}

void STM_NeedleStatus() {
  bool SensorOT = (digitalRead(PIN_OT) == LOW);
  bool SensorUT = (digitalRead(PIN_UT) == LOW);

  switch (StatusNeedle) {
    case UNKNOWN_Triggered:
      
      if (SensorOT) {
        StatusNeedle = OT_Triggered;
      } else if (SensorUT) {
        StatusNeedle = UT_Triggered;
      } else {
        StatusNeedle = UNKNOWN_Hold;
      }

      break;
    case UNKNOWN_Hold:

      if (SensorOT) {
        StatusNeedle = OT_Triggered;
      } else if (SensorUT) {
        StatusNeedle = UT_Triggered;
      }

      break;
    case OT_Triggered:

      if (SensorOT) {
        StatusNeedle = OT_Hold;
      } else if (SensorUT) {
        StatusNeedle = UT_Triggered;
      } else {
        StatusNeedle = UNKNOWN_Triggered;
      }

      break;
    case OT_Hold:
      
      if (SensorUT) {
        StatusNeedle = UT_Triggered;
      } else if (!SensorOT) {
        StatusNeedle = UNKNOWN_Triggered;
      }

      break;
    case UT_Triggered:

      if (SensorUT) {
        StatusNeedle = UT_Hold;
      } else if (SensorOT) {
        StatusNeedle = OT_Triggered;
      } else {
        StatusNeedle = UNKNOWN_Triggered;
      }
      break;
    case UT_Hold:
      if (SensorOT) {
        StatusNeedle = OT_Triggered;
      } else if (!SensorUT) {
        StatusNeedle = UNKNOWN_Triggered;
      }
      break;
  }

}

void loop() {
  // static NeedleStatus LastStatusNeedle = UNKNOWN_Hold;
  unsigned long CurrentMillis = millis();
  static unsigned long LastUTMillis = CurrentMillis;
  float SecsPerStitch;

  STM_NeedleStatus();

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
  
  STM_Main();
  stepper.loop();
  
}


void MotorOff() {
  if (!isPowerOff) {
    stepper.powerOff();
    isPowerOff = true;
  }
}

void MotorOn() {
  if (isPowerOff) {
    stepper.powerOn();
    isPowerOff = false;
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
    target_speed = max_speed;    
  } else {
    target_speed = map(val, HAL_SENSOR_FULL_THROTTLE_VALUE, HAL_SENSOR_STOP_VALUE, max_speed, 0);
  }
  
}

void setupStopValue() {  
  int val = analogRead(PIN_POT);
  HAL_SENSOR_STOP_VALUE = val + 10;
}

void setupStepper() {
  stepper.begin(PIN_PUL, PIN_DIR);
  stepper.setAcceleration(acceleration);
  stepper.setEnablePin(PIN_ENA);
  MotorOff();
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

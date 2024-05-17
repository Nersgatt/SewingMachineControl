#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

#include <config.h>

int HAL_SENSOR_STOP_VALUE = 530;

int target_speed = 0;
int last_target_speed = 0;
boolean isPowerOff = false;

enum MachineStatus {STOP, STARTING, RUNNING, STOPPING};

MachineStatus Status = STOP;
const String StatusText[4] = {"STOP", "STARTING", "RUNNING", "STOPPING"};

ContinuousStepper<StepperDriver, ToneTicker> stepper;

void setupStepper();
void setupStopValue();
void setupTimer1() ;

void STM_Main();
void MotorOff();
void MotorOn();

void setup() {

  #ifdef DEBUG_INFO
  Serial.begin(9600);
  #endif

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

void loop() {
  

  #ifdef DEBUG_INFO
    Serial.print("Status ");
    Serial.println(StatusText[Status]);
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

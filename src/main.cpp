#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>
#include <setup.h>

const int PIN_PUL = 12;
const int PIN_DIR = 6;
const int PIN_ENA = A1;
const int PIN_POT = A6;
const int PIN_NEEDLE_OT = 8;
const int PIN_NEEDLE_UT = 2;

const int max_speed = 12000;
const int acceleration = 30000;
const int HAL_SENSOR_FULL_THROTTLE_VALUE = 860;

int HAL_SENSOR_STOP_VALUE = 530;

int target_speed = 0;
int last_target_speed = 0;
boolean isPowerOff = false;

ContinuousStepper<StepperDriver, ToneTicker> stepper;

enum NeedlePosition {npOT, npUT, npInBetween};
const char* NeedlePositionStr[] = {"OT", "UT", "in between"};

void setupStepper();
void setupStopValue();
void MotorOff();
void MotorOn();
NeedlePosition getNeedlePosition();

void setup() {


  cli(); // disable interrupts during setup 
  Serial.begin(9600);

  setupTimer1();
  setupStepper();
  setupStopValue();
  MotorOff();  
  sei(); // re-enable interrupts

}

void loop() {
  
  if ((target_speed == 0) and (!stepper.isSpinning())) {
    MotorOff();
  } else if ((target_speed > 0) and (!stepper.isSpinning())) {
    MotorOn();
  }

  if (last_target_speed != target_speed) {
    // Serial.println(target_speed);    
    last_target_speed = target_speed;
    stepper.spin(target_speed * -1);    
  }

  Serial.println(NeedlePositionStr[getNeedlePosition()]);

  stepper.loop();
  
}

NeedlePosition getNeedlePosition() {
  if (digitalRead(PIN_NEEDLE_OT) == HIGH) {
    return npOT;
  } else if (digitalRead(PIN_NEEDLE_UT) == HIGH) {
    return npUT;
  } else {
    return npInBetween;
  }
}


void MotorOff() {
  if (!isPowerOff) {
    stepper.powerOff();
    isPowerOff = true;
    // Serial.println("Power off!");
  }
}

void MotorOn() {
  if (isPowerOff) {
    stepper.powerOn();
    isPowerOff = false;
    // Serial.println("Power on!");
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
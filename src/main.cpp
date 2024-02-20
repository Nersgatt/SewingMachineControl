#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

const int PIN_PUL = 12;
const int PIN_DIR = 6;
const int PIN_POT = A6;

int HAL_SENSOR_STOP_VALUE = 530;
const int HAL_SENSOR_FULL_THROTTLE_VALUE = 860;

const int max_speed = 12000;
const int acceleration = 2000;
int target_speed = 0;
int last_target_speed = 0;
boolean isPowerOff = true;

ContinuousStepper<StepperDriver, ToneTicker> stepper;

void setupTimer1();
void setupStepper();
void setupStopValue();
void MotorOff();
void MotorOn();

void setup() {
  Serial.begin(9600);

  cli(); // disable interrupts during setup 
  setupTimer1();
  setupStepper();
  setupStopValue();
  sei(); // re-enable interrupts

}

void loop() {
  
  if (last_target_speed != target_speed) {
    last_target_speed = target_speed;

    if (target_speed <= 0) {
      MotorOff();
    } else {
      if (isPowerOff) {
        MotorOn();
      }
      stepper.spin(target_speed * -1);    
    }
  }

  stepper.loop();
  
}

void MotorOff() {
  stepper.powerOff();
  isPowerOff = true;
  Serial.println("Power off!");
}

void MotorOn() {
  stepper.powerOn();
  isPowerOff = false;
  Serial.println("Power on!");
}

void setupStopValue() {  
  int val = analogRead(PIN_POT);
  HAL_SENSOR_STOP_VALUE = val + 10;
}

void setupTimer1() {
  noInterrupts();
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 2 Hz (16000000/((31249+1)*256))
  OCR1A = 31249;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 256
  TCCR1B |= (1 << CS12);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();
}

void setupStepper() {
  stepper.begin(PIN_PUL, PIN_DIR);
  stepper.setAcceleration(acceleration);    
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

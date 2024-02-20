#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

const int PIN_PUL = 12;
const int PIN_DIR = 6;
const int PIN_ENA = A1;
const int PIN_POT = A6;
const int PIN_STATUS_MOTOR = 2;

int HAL_SENSOR_STOP_VALUE = 530;
const int HAL_SENSOR_FULL_THROTTLE_VALUE = 860;

const int max_speed = 12000;
const int acceleration = 20000;
// const int deceleration = 20000;

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

  pinMode(PIN_STATUS_MOTOR, OUTPUT);

  setupTimer1();
  setupStepper();
  setupStopValue();
  MotorOff();
  sei(); // re-enable interrupts

}

void loop() {
  
  // Serial.println(stepper.isSpinning());
  if ((target_speed == 0) and (!stepper.isSpinning())) {
    MotorOff();
  } else if ((target_speed > 0) and (!stepper.isSpinning())) {
    MotorOn();
  }

  if (last_target_speed != target_speed) {
    Serial.println(target_speed);    
    last_target_speed = target_speed;
    stepper.spin(target_speed * -1);    
  }

  stepper.loop();
  
}

void MotorOff() {
  if (!isPowerOff) {
    stepper.powerOff();
    digitalWrite(PIN_STATUS_MOTOR, LOW);
    isPowerOff = true;
    Serial.println("Power off!");
  }
}

void MotorOn() {
  if (isPowerOff) {
    stepper.powerOn();
    digitalWrite(PIN_STATUS_MOTOR, HIGH);
    isPowerOff = false;
    Serial.println("Power on!");
  }
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

  // 4 Hz (16000000/((15624+1)*256))
  OCR1A = 15624;
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
  stepper.setEnablePin(PIN_ENA);
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

#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

#include <config.h>

int HAL_SENSOR_STOP_VALUE = 530;

int target_speed = 0;
int last_target_speed = 0;
boolean isPowerOff = false;

ContinuousStepper<StepperDriver, ToneTicker> stepper;

void setupStepper();
void setupStopValue();
void setupTimer1() ;

void MotorOff();
void MotorOn();

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
    last_target_speed = target_speed;
    stepper.spin(target_speed * -1);    
  }


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

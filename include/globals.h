#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

#include <LiquidCrystal_I2C.h>

extern ContinuousStepper<StepperDriver, ToneTicker> stepper;
extern int target_speed;

extern int HAL_SENSOR_STOP_VALUE;

extern LiquidCrystal_I2C lcd;
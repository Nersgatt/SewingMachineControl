#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

#include <globals.h>

ContinuousStepper<StepperDriver, ToneTicker> stepper;
int target_speed = 0;
boolean isPowerOff = false;

int HAL_SENSOR_STOP_VALUE = 530;

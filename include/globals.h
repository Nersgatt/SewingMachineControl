#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

extern ContinuousStepper<StepperDriver, ToneTicker> stepper;
extern int target_speed;
extern boolean isPowerOff;

extern int HAL_SENSOR_STOP_VALUE;


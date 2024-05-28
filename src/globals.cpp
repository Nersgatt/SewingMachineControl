#include <Arduino.h>
#include <ContinuousStepper.h>
#include <ContinuousStepper/Tickers/Tone.hpp>

#include <LiquidCrystal_I2C.h>

#include <globals.h>

ContinuousStepper<StepperDriver, ToneTicker> stepper;
int target_speed = 0;

int HAL_SENSOR_STOP_VALUE = 530;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

unsigned long CurrentMillis = 0;
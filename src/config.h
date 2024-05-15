#include <Arduino.h>

const int PIN_PUL = 12;
const int PIN_DIR = 6;
const int PIN_ENA = A1;
const int PIN_POT = A6;
const int PIN_NEEDLE_OT = 8;
const int PIN_NEEDLE_UT = 2;

const int max_speed = 12000;
const int acceleration = 30000;
const int HAL_SENSOR_FULL_THROTTLE_VALUE = 860;
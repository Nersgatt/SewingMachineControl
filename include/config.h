#include <Arduino.h>

const int PIN_PUL = 12; // PULSE-Pin am Motortreiber
const int PIN_DIR = 6;  // DIR-Pin am Motortreiber
const int PIN_ENA = A1; // ENA-Pin an Motortreiber
const int PIN_POT = A6; // Analoger Ausgang des KY-024
const int PIN_UT = 2;   // PIN für UT-Sensor
const int PIN_OT = 8;   // PIN für OT-Sensor

const int max_speed = 12000;                    // Höchstgeschwindigkeit
const int acceleration = 30000;                 // Beschleunigung
const int HAL_SENSOR_FULL_THROTTLE_VALUE = 860; // Vollgas-Wert des HAL-Sensors

#define DEBUG_INFO  // Achtung: Wenn DEBUG-Infos ausgegeben werden, läuft die Maschine deutlich langsamer, da stepper.loop (zeitlich gesehen) seltener aufgerufen wird

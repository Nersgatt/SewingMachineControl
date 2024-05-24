#include <Arduino.h>

#define PIN_PUL 12 // PULSE-Pin am Motortreiber
#define PIN_DIR 6  // DIR-Pin am Motortreiber
#define PIN_ENA A1 // ENA-Pin an Motortreiber
#define PIN_POT A6 // Analoger Ausgang des KY-024
#define PIN_UT 2   // PIN für UT-Sensor
#define PIN_OT 8   // PIN für OT-Sensor

#define MAX_SPEED 12000                    // Höchstgeschwindigkeit
#define ACC 30000                          // Beschleunigung
#define HAL_SENSOR_FULL_THROTTLE_VALUE 860 // Vollgas-Wert des HAL-Sensors

#define DEBUG_INFO  // Achtung: Wenn DEBUG-Infos ausgegeben werden, läuft die Maschine deutlich langsamer, da stepper.loop (zeitlich gesehen) seltener aufgerufen wird

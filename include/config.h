#include <Arduino.h>

#define PIN_PUL 12              // PULSE-Pin am Motortreiber
#define PIN_DIR 6               // DIR-Pin am Motortreiber
#define PIN_ENA A1              // ENA-Pin an Motortreiber
#define PIN_POT A6              // Analoger Ausgang des KY-024
#define PIN_UT 2                // PIN für UT-Sensor
#define PIN_OT 8                // PIN für OT-Sensor
#define PIN_MOVE_NEEDLE 3       // PIN um die Nadel hoch oder runter zu stellen (Taster)
#define PIN_ONE_STITCH 4        // PIN um einen Stich zu nähen (Taster)
#define PIN_TOOGLE_POSITION 5   // PIN um die gewünschte Stop-Position der Nadel einzustellen (Taster)

#define MAX_SPEED 12000                    // Höchstgeschwindigkeit
#define POSITIONING_SPEED 2000             // Geschwindigkeit, mit der die OT oder UT-Position angefahren wird;
#define MAX_POSITIONING_DELAY 1000         // Dauer, die ein Positionierungvorgang maximal dauern darf. Danach geht die Maschine in den Fehlerstatus
#define ACC 30000                          // Beschleunigung
#define HAL_SENSOR_FULL_THROTTLE_VALUE 860 // Vollgas-Wert des HAL-Sensors

#define BOUNCE_DELAY 50 // DELAY für das Debouncing der Buttons
#define HOLD_DELAY 1000 // DELAY, nach dem der Button in den Status "HOLD" geht, um einen langen Buttonpress zu erkennen


#define DEBUG_INFO  // Achtung: Wenn DEBUG-Infos ausgegeben werden, läuft die Maschine deutlich langsamer, da stepper.loop (zeitlich gesehen) seltener aufgerufen wird

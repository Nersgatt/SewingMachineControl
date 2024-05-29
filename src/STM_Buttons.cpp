#include <Arduino.h>
#include <config.h>
#include <globals.h>
#include <STM_Buttons.h>

ButtonStatus Status_BTN_NeedlePosition = RESET;
ButtonStatus Status_BTN_MoveNeedle = RESET;
ButtonStatus Status_BTN_OneStitch = RESET;


void STM_BTN_NeedlePosition() {
  //Almost every state needs these lines, so I'll put it outside the State Machine
  int val_s1 = digitalRead(PIN_TOOGLE_POSITION);
  unsigned long t_s1 = 0;
  static unsigned long t_0_s1 = 0;

  //State Machine Section
  switch (Status_BTN_NeedlePosition) {
    case RESET: //RESET!
      //Catch all "home base" for the State MAchine
      Status_BTN_NeedlePosition = WAIT;
    break;

    case WAIT: //WAIT
      //Wait for the switch to go low
      if (val_s1 == LOW) {Status_BTN_NeedlePosition = ARMING;}
    break;

    case ARMING: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s1 = CurrentMillis;
      Status_BTN_NeedlePosition = ARMED;
    break;

    case ARMED: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s1 = CurrentMillis;
      if (t_s1 - t_0_s1 > BUTTON_BOUNCE_DELAY) {Status_BTN_NeedlePosition = DRAWN;}
      if (val_s1 == HIGH) {Status_BTN_NeedlePosition = RESET;}
    break;

    case DRAWN: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s1 = CurrentMillis;
      if (val_s1 == HIGH) {Status_BTN_NeedlePosition = TRIGGERED;}
      if (t_s1 - t_0_s1 > BUTTON_HOLD_DELAY) {Status_BTN_NeedlePosition = HOLD;}
    break;

    case TRIGGERED: //TRIGGERED!
      //reset the State Machine
      Status_BTN_NeedlePosition = RESET;
    break;

    case HOLD: //HOLD!
      //proceed to LOW WAIT
      Status_BTN_NeedlePosition = LOWWAIT;
    break;

    case LOWWAIT: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s1 == HIGH) {Status_BTN_NeedlePosition = RESET;}
    break;
  }
  
}

void STM_BTN_MoveNeedle() {
  //Almost every state needs these lines, so I'll put it outside the State Machine
  int val_s1 = digitalRead(PIN_MOVE_NEEDLE);
  unsigned long t_s1 = 0;
  static unsigned long t_0_s1 = 0;

  //State Machine Section
  switch (Status_BTN_MoveNeedle) {
    case RESET: //RESET!
      //Catch all "home base" for the State MAchine
      Status_BTN_MoveNeedle = WAIT;
    break;

    case WAIT: //WAIT
      //Wait for the switch to go low
      if (val_s1 == LOW) {Status_BTN_MoveNeedle = ARMING;}
    break;

    case ARMING: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s1 = CurrentMillis;
      Status_BTN_MoveNeedle = ARMED;
    break;

    case ARMED: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s1 = CurrentMillis;
      if (t_s1 - t_0_s1 > BUTTON_BOUNCE_DELAY) {Status_BTN_MoveNeedle = DRAWN;}
      if (val_s1 == HIGH) {Status_BTN_MoveNeedle = RESET;}
    break;

    case DRAWN: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s1 = CurrentMillis;
      if (val_s1 == HIGH) {Status_BTN_MoveNeedle = TRIGGERED;}
      if (t_s1 - t_0_s1 > BUTTON_HOLD_DELAY) {Status_BTN_MoveNeedle = HOLD;}
    break;

    case TRIGGERED: //TRIGGERED!
      //reset the State Machine
      Status_BTN_MoveNeedle = RESET;
    break;

    case HOLD: //HOLD!
      //proceed to LOW WAIT
      Status_BTN_MoveNeedle = LOWWAIT;
    break;

    case LOWWAIT: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s1 == HIGH) {Status_BTN_MoveNeedle = RESET;}
    break;
  }
  
}

void STM_BTN_OneStitch() {
  //Almost every state needs these lines, so I'll put it outside the State Machine
  int val_s1 = digitalRead(PIN_ONE_STITCH);
  unsigned long t_s1 = 0;
  static unsigned long t_0_s1 = 0;

  //State Machine Section
  switch (Status_BTN_OneStitch) {
    case RESET: //RESET!
      //Catch all "home base" for the State MAchine
      Status_BTN_OneStitch = WAIT;
    break;

    case WAIT: //WAIT
      //Wait for the switch to go low
      if (val_s1 == LOW) {Status_BTN_OneStitch = ARMING;}
    break;

    case ARMING: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s1 = CurrentMillis;
      Status_BTN_OneStitch = ARMED;
    break;

    case ARMED: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s1 = CurrentMillis;
      if (t_s1 - t_0_s1 > BUTTON_BOUNCE_DELAY) {Status_BTN_OneStitch = DRAWN;}
      if (val_s1 == HIGH) {Status_BTN_OneStitch = RESET;}
    break;

    case DRAWN: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s1 = CurrentMillis;
      if (val_s1 == HIGH) {Status_BTN_OneStitch = TRIGGERED;}
      if (t_s1 - t_0_s1 > BUTTON_HOLD_DELAY) {Status_BTN_OneStitch = HOLD;}
    break;

    case TRIGGERED: //TRIGGERED!
      //reset the State Machine
      Status_BTN_OneStitch = RESET;
    break;

    case HOLD: //HOLD!
      //proceed to LOW WAIT
      Status_BTN_OneStitch = LOWWAIT;
    break;

    case LOWWAIT: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s1 == HIGH) {Status_BTN_OneStitch = RESET;}
    break;
  }
  
}
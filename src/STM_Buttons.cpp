#include <Arduino.h>
#include <config.h>
#include <globals.h>
#include <STM_Buttons.h>

ButtonStatus Status_BTN_NeedlePosition = bsRESET;
ButtonStatus Status_BTN_MoveNeedle = bsRESET;
ButtonStatus Status_BTN_OneStitch = bsRESET;


void STM_BTN_NeedlePosition() {
  //Almost every state needs these lines, so I'll put it outside the State Machine
  int val_s1 = digitalRead(PIN_TOOGLE_POSITION);
  unsigned long t_s1 = 0;
  static unsigned long t_0_s1 = 0;

  //State Machine Section
  switch (Status_BTN_NeedlePosition) {
    case bsRESET: //RESET!
      //Catch all "home base" for the State MAchine
      Status_BTN_NeedlePosition = bsWAIT;
    break;

    case bsWAIT: //WAIT
      //Wait for the switch to go low
      if (val_s1 == LOW) {Status_BTN_NeedlePosition = bsARMING;}
    break;

    case bsARMING: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s1 = CurrentMillis;
      Status_BTN_NeedlePosition = bsARMED;
    break;

    case bsARMED: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s1 = CurrentMillis;
      if (t_s1 - t_0_s1 > BUTTON_BOUNCE_DELAY) {Status_BTN_NeedlePosition = bsDRAWN;}
      if (val_s1 == HIGH) {Status_BTN_NeedlePosition = bsRESET;}
    break;

    case bsDRAWN: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s1 = CurrentMillis;
      if (val_s1 == HIGH) {Status_BTN_NeedlePosition = bsTRIGGERED;}
      if (t_s1 - t_0_s1 > BUTTON_HOLD_DELAY) {Status_BTN_NeedlePosition = bsHOLD;}
    break;

    case bsTRIGGERED: //TRIGGERED!
      //reset the State Machine
      Status_BTN_NeedlePosition = bsRESET;
    break;

    case bsHOLD: //HOLD!
      //proceed to LOW WAIT
      Status_BTN_NeedlePosition = bsLOWWAIT;
    break;

    case bsLOWWAIT: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s1 == HIGH) {Status_BTN_NeedlePosition = bsRESET;}
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
    case bsRESET: //RESET!
      //Catch all "home base" for the State MAchine
      Status_BTN_MoveNeedle = bsWAIT;
    break;

    case bsWAIT: //WAIT
      //Wait for the switch to go low
      if (val_s1 == LOW) {Status_BTN_MoveNeedle = bsARMING;}
    break;

    case bsARMING: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s1 = CurrentMillis;
      Status_BTN_MoveNeedle = bsARMED;
    break;

    case bsARMED: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s1 = CurrentMillis;
      if (t_s1 - t_0_s1 > BUTTON_BOUNCE_DELAY) {Status_BTN_MoveNeedle = bsDRAWN;}
      if (val_s1 == HIGH) {Status_BTN_MoveNeedle = bsRESET;}
    break;

    case bsDRAWN: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s1 = CurrentMillis;
      if (val_s1 == HIGH) {Status_BTN_MoveNeedle = bsTRIGGERED;}
      if (t_s1 - t_0_s1 > BUTTON_HOLD_DELAY) {Status_BTN_MoveNeedle = bsHOLD;}
    break;

    case bsTRIGGERED: //TRIGGERED!
      //reset the State Machine
      Status_BTN_MoveNeedle = bsRESET;
    break;

    case bsHOLD: //HOLD!
      //proceed to LOW WAIT
      Status_BTN_MoveNeedle = bsLOWWAIT;
    break;

    case bsLOWWAIT: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s1 == HIGH) {Status_BTN_MoveNeedle = bsRESET;}
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
    case bsRESET: //RESET!
      //Catch all "home base" for the State MAchine
      Status_BTN_OneStitch = bsWAIT;
    break;

    case bsWAIT: //WAIT
      //Wait for the switch to go low
      if (val_s1 == LOW) {Status_BTN_OneStitch = bsARMING;}
    break;

    case bsARMING: //ARMING!
      //Record the time and proceed to ARMED
      t_0_s1 = CurrentMillis;
      Status_BTN_OneStitch = bsARMED;
    break;

    case bsARMED: //ARMED
      //Check to see if the proper has delay has passed.  If a bounce occures then RESET
      t_s1 = CurrentMillis;
      if (t_s1 - t_0_s1 > BUTTON_BOUNCE_DELAY) {Status_BTN_OneStitch = bsDRAWN;}
      if (val_s1 == HIGH) {Status_BTN_OneStitch = bsRESET;}
    break;

    case bsDRAWN: //DRAWN
      //If switch goes HIGH, then TRIGGER. Also check timer for a "Long Pess"
      t_s1 = CurrentMillis;
      if (val_s1 == HIGH) {Status_BTN_OneStitch = bsTRIGGERED;}
      if (t_s1 - t_0_s1 > BUTTON_HOLD_DELAY) {Status_BTN_OneStitch = bsHOLD;}
    break;

    case bsTRIGGERED: //TRIGGERED!
      //reset the State Machine
      Status_BTN_OneStitch = bsRESET;
    break;

    case bsHOLD: //HOLD!
      //proceed to LOW WAIT
      Status_BTN_OneStitch = bsLOWWAIT;
    break;

    case bsLOWWAIT: //LOW WAIT
      //wait for switch to go back HIGH, then reset
      if (val_s1 == HIGH) {Status_BTN_OneStitch = bsRESET;}
    break;
  }
  
}
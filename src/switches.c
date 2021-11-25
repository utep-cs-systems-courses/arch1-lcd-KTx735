#include <msp430.h>
#include "switches.h"
#include "led.h"
#include "stateMachines.h"

char button_state;
char switch_state_down, switch_state_changed; /* effectively boolean */

static char 
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);	/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);	/* if switch down, sense up */
  return p2val;
}

void 
switch_init()			/* setup switch */
{  
  P2REN |= SWITCHES;		/* enables resistors for switches */
  P2IE |= SWITCHES;		/* enable interrupts from switches */
  P2OUT |= SWITCHES;		/* pull-ups for switches */
  P2DIR &= ~SWITCHES;		/* set switches' bits for input */
  switch_update_interrupt_sense();
  led_update();
}

void
switch_interrupt_handler(){
  char p2val = checkSwitches();
  //set the value of state to the corresponding switch that is pressed
  char state = (p2val & SW1) ? 0 : 1;
  if (state == 0) state = (p2val & SW2) ? 0 : 2;
  if (state == 0) state = (p2val & SW3) ? 0 : 3;
  if (state == 0) state = (p2val & SW4) ? 0 : 4;
  stateChange(state);
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    switch_interrupt_handler();/* single handler for all switches */
  }
}

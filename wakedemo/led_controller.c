#include <msp430.h>
#include "led.h"

//This will control the LEDs
//Order: Turn off, Green on, Red on, Both on
void led_controller(int toggle){
  switch(toggle){
  case 0:
    P1OUT &= ~LEDS;
    break;
  case 1:
    P1OUT |= LED_GREEN;
    P1OUT &= ~LED_RED;
    break;
  case 2:
    P1OUT &= ~LED_GREEN;
    P1OUT |= LED_RED;
    break;
  default:
    P1OUT |= LEDS;
    break;
  }
}

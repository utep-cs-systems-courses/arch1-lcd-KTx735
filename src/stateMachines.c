#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"
#include "switches.h"

char button_state;
char lightstate = 0;
char state = 0;

//Red at 50%
//Order: on, off
void redAt50()
{
  switch (lightstate) {
  case 0:
    red_on = 1;
    state = 1;
    break;
  case 1:
    red_on = 0;
    state = 0;
    break;
  }
  led_update();
}

//Red at 75%
//Order: on, on, off
void redAt75()
{
  switch (lightstate) {
  case 0:
    red_on = 1;
    state = 1;
    break;
  case 1:
    red_on = 1;
    state = 2;
    break;
  case 2:
    red_on = 0;
    state = 0;
    break;
  }
  led_update();
}  

//Red at 25%
//Order: off, off, on
void redAt25()
{
  switch (lightstate) {
  case 0:
    red_on = 0;
    state = 1;
    break;
  case 1:
    red_on = 0;
    state = 2;
    break;
  case 2:
    red_on = 1;
    state = 0;
    break;
  }
  led_update();
}

//changeLight uses the intensity - 50, 75 and 25
void changeLight()
{
  switch (lightstate) {
  case 0:
    redAt50();
    redAt50();
    state = 1;
    break;
  case 1: 
    redAt75();
    redAt75();
    redAt75();
    state = 2;
    break;
  case 2:
    redAt25();
    redAt25();
    redAt25();
    dim_state = 0;
    break;
  }
}

//Turn green on and turn red off
void greenOn()
{
  green_on = 1;
  red_on = 0;
  led_update();
}

//Turn red on and turn green off
void redOn()
{
  red_on = 1;
  green_on = 0;
  led_update();
}

//Frequency is raised
void soundUp()
{
  static long cycle = 5000;
  buzzer_set_period(cycle);
  cycle = cycle - 10;
  if(cycle == 0){
    cycle = 5000;
  }
}

//Frequency is lowered
void soundDown()
{
  static long cycle = 0;
  buzzer_set_period(cycle);
  cycle = cycle + 10;
  if(cycle == 5000){
    cycle = 0;
  }
}

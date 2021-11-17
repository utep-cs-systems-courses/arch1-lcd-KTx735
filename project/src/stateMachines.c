#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"
#include "switches.h"

char button_state;
char lightstate = 0;
char state = 0;

void redAt50()
{
  switch(state){
  case 0:
    red_on = 1;
    state = 1;
    break;
  case 2:
    red_on = 0;
    state = 0;
    break;
  }
  led_update();
}

void redAt75()
{
  switch(state){
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

void redAt25()
{
  switch(state){
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

void greenOn()
{
  green_on = 1;
  red_on = 0;
  led_update();
}

void changeLight()
{
  static char lightState = 0;
  switch(lightState){
  case 0:
    redAt50();
    redAt50();
    lightState = 1;
    break;
  case 1:
    redAt75();
    redAt75();
    redAt75();
    lightState = 2;
    break;
  case 2:
    redAt25();
    redAt25();
    redAt25();
    lightState = 0;
      break;
  }
}

//Complex test
/*
void soundFromHell()
{
  static char note_state = 0;
  switch(note_state){
  case 0:
    buzzer_set_period(2551);
      note_state++;
      break;
  case 2:
    buzzer_set_period(3822);
    note_state++;
    break;
  case 6:
    buzzer_set_period(5000);
    note_state++;
    break;
  case 10:
    buzzer_set_period(3405);
    note_state++;
    break;
  case 11:
    buzzer_set_period(5000);
    note_state++;
    break;
  case 15:
    buzzer_set_period(2551);
    note_state++;
    break;
  case 16:
    buzzer_set_period(3405);
    note_state = 0;
    break;
  }
}
*/

void soundUp()
{ //frequency is raised
  static long cycle = 5000;
  buzzer_set_period(cycle);
  cycle = cycle - 10;
  if(cycle == 0){
    cycle = 5000;
  }
}

void soundDown()
{ // frequency is lowered
  static long cycle = 0;
  buzzer_set_period(cycle);
  cycle = cycle + 10;
  if(cycle == 5000){
    cycle = 0;
  }
}

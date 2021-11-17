#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "switches.h"
#include "buzzer.h"

char button_state = 0;

void
__interrupt_vec(WDT_VECTOR) WDT(){
  static char blink_count = 0;
  
  if(++blink_count == 1 && button_state == 0){ //Sound up
    soundUp();
    blink_count = 0;
  }
  else if(button_state == 1){
    soundDown();
  }
  else if(button_state == 2){ //Light green on
    buzzer_set_period(0);
    greenOn();
  }
  else if(blink_count == 3 && button_state == 3){ //Lights change
    buzzer_set_period(0);
    changeLight();
    blink_count = 0;
  }
  else if(button_state == 4){
    redAt50();
    blink_count = 0;
  }
}

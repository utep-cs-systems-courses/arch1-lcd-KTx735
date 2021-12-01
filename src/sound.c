#include <msp430.h>
#include "sound.h"
#include "buzzer.h"

void sound()
{
  while(1){
  switch(state)
    {
    case 0:
      cycle = 5000;
      state = 1;
      break;
    case 1:
      cycle = 4000;
      state = 2;
      break;
    case 2:
      cycle = 2000;
      state = 3;
      break;
    case 3:
      cycle = 1000;
      state = 0;
      break;
    }
  buzzer_set_period(cycle);
  }
}

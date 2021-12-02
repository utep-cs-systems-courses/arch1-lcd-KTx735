#include <msp430.h>
#include "sound.h"
#include "buzzer.h"

void sound()
{
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
	state = 0;
	break;
       default:
	cycle = 1000;
	state = 0;
	break;
      }
    buzzer_set_period(cycle);
}

#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "led.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

// #define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

//Declare to avoid errors
short redrawScreen = 0;
int switches = 0;
int button = 0;
int cpuOff = 0;

void draw_stuff();
void update_shape();
void clearlcd();
void update_clear();

void counter();
void blinkCounter();
void blinkUpdate();

void
led_init()
{
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
}


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
}

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
  if (switches & SW4)
    {
      redrawScreen = 1;
      button = 4;
    }
  if(switches & SW3)
    {
      redrawScreen = 1;
      button = 3;
    }
  if (switches & SW2)
    {
      redrawScreen = 1;
      button = 2;
    }
  if(switches & SW1)
    {
      redrawScreen = 1;
      button = 1;
    }
}

//This is trash code. I'll levae this just in case.
// axis zero for col, axis 1 for row
//short drawPos[2] = {10,10}, controlPos[2] = {10,10};
//short velocity[2] = {3,8}, limits[2] = {screenWidth-36, screenHeight-8};
//short redrawScreen = 1;
//u_int controlFontColor = COLOR_GREEN;

void time_update();

//Check later
void wdt_c_handler()
{
  time_update();
}
  
void update_shape();

//Copy from previous main.c
//Can be incorporated in here
void main()
{
  configureClocks();
  lcd_init();
  switch_init();
  led_init();
  buzzer_init();
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);              /**< GIE (enable interrupts) */

  clearlcd();

  while(1)
    {
      if(redrawScreen)
	{
	  redrawScreen = 0;
	  update_shape();
	}
      //This will turn the CPU off
      or_sr(0x10);
    }
}

//Use 5x7 for testing purpose
//Change later to 8x12
void
draw_stuff()
{
  drawString5x7(10, 20, "LCD", COLOR_WHITE, COLOR_RED);
  drawString5x7(10, 40, "Draw", COLOR_WHITE, COLOR_RED);
  drawString5x7(10, 60, "Sound", COLOR_WHITE, COLOR_RED);
  drawString5x7(10, 80, "Clear", COLOR_WHITE, COLOR_RED);
  //drawString8x12(10,20, "LCD", COLOR_WHITE, COLOR_RED);
  //drawString8x12(10,40, "Draw", COLOR_WHITE, COLOR_RED);
  //drawString8x12(10,60, "Clear", COLOR_WHITE, COLOR_RED);
  //drawString8x12(10,80, "Clear", COLOR_WHITE, COLOR_RED);
  //drawString11x16(10, 20, "LCD", COLOR_WHITE, COLOR_RED);
  //drawString11x16(10, 40, "Draw", COLOR_WHITE, COLOR_RED);
  //drawString11x16(10, 60, "Sound", COLOR_WHITE, COLOR_RED);
  // drawString11x16(10, 80, "Clear", COLOR_WHITE, COLOR_RED);
}

//More like blink
unsigned char move = 50;

void
update_shape()
{
  switch (button){
  case 1:
    if(move <= 100){
      if(move == 50){
	clearScreen(COLOR_BLUE);
      }
      drawDiamond(move, COLOR_RED);
      move++;
    }
    else{
      clearScreen(COLOR_BLUE);
      move = 50;
    }
    break;
  case 2:
    if(move >= 50 && move <= 100){
      drawDiamond(move, COLOR_RED);
      move--;
    }
    else{
      clearScreen(COLOR_BLUE);
      move = 100;
    }
  case 3:
    clearScreen(COLOR_BLUE);
    break;
  default:
    clearlcd();
    break;
  }
}

void
clearlcd()
{
  clearScreen(COLOR_BLUE);
  draw_stuff();
}

int time_counter = 0;

void
normalTime()
{
  time_counter ++;
  //10 sec
  if (time_counter >= 15) {
    if(!cpuOff){
      time_counter = 0;
      redrawScreen = 1;
    }
    else{
      time_counter = 0;
      redrawScreen = 0;
      cpuOff = 0;
    }
  }
}

int second_counter = 0;
int blink_counter = 0;
int blink_limit = 0;
int dim = 0;

//Probably shold change for a real song
//Nah, leave it as ugly sound
//Probably need to change notes because it sounds all of them sound simillar
int noise[] = {5405, 6802, 5102, 6060, 6802, 4049, 5405, 5102, 3610};

void blinkUpdate()
{
  blink_counter++;
  if(blink_counter >= blink_limit && noise[blink_limit] > 0)
    {
      blink_counter = 0;
      buzzer_set_period(noise[blink_limit]);
      if(!dim)
	{
	  led_controller(1);
	}
      else if(dim && blink_limit == 50)
	{
	  led_controller(3);
	}
      else
	{
	  led_controller(2);
	}
    }
  else
    {
      led_controller(0);
    }
  counter();
}

//When the state is 4, it will stop
void blinkCounter()
{
  second_counter++;
  if (second_counter >= 3000) {
    second_counter = 0;
    if(!dim){
      blink_limit++;
      if (blink_limit >= 50){
	dim = 1;
      }
    }
    else{
      blink_limit--;
      if(blink_limit <= -1){
	button = 4;
      }
    }
  }
}

//Counter for blinks
//This will move slow
void
counter()
{
  second_counter++;
  if(second_counter >= 120)
    {
      if(!cpuOff)
	{
	  second_counter = 0;
	  redrawScreen = 1;
	}
      else
	{
	  second_counter = 0;
	  redrawScreen = 0;
	  cpuOff = 0;
	}
    }
}

//This is a restart, leds and buzzer restart
void
restart_val()
{
  second_counter = 0;
  blink_counter = 0;
  blink_limit = 0;
  dim = 0;

  buzzer_set_period(0);
  led_controller(0);
  move = 50;
  cpuOff = 1;
  if(button == 4){
    clearlcd();
  }
  else{
    clearScreen(COLOR_BLUE);
  }
  button = 0;
}

void update_clear(){
  second_counter = 0;
  blink_counter = 0;
  blink_limit = 0;
  dim = 0;

  if(button == 0 || button > 2){
    move = 50;
  }
  button = 0;
  cpuOff = 1;
  clearScreen(COLOR_BLUE);
  buzzer_set_period(0);
  led_controller(0);
}

void time_update()
{
  switch(button){
  case 1:
  case 2:
    counter();
    break;
  case 3:
    blinkUpdate();
    break;
  case 4:
    restart_val();
    break;
  }
}

/* Switch on S2 */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "sound.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 

#define LED BIT6		/* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

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

int switches = 0;
short update = 1;
int cycle = 5000;
char state = 0;

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}


// axis zero for col, axis 1 for row
short drawPos[2] = {10,10}, controlPos[2] = {10,10};
short velocity[2] = {3,8}, limits[2] = {screenWidth-36, screenHeight-8};

short redrawScreen = 1;
u_int controlFontColor = COLOR_GREEN;

void wdt_c_handler()
{
  static int secCount = 0;

  secCount ++;
  if (secCount >= 25) {		/* 10/sec */
    secCount = 0;
    redrawScreen = 1;
    update = 1;
  }
}
  
void update_shape();
void squareface(int step, int color);
void button1();
void button2();
void button3();
void button4();
void defaultBehaviour();
void buttonSelected();

void main()
{
  
  P1DIR |= LED;		/**< Green led on when CPU on */
  P1OUT |= LED;
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();
  
  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */
  
  clearScreen(COLOR_BLUE);
  while (1) {			/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      buttonSelected();
    }
    P1OUT &= ~LED;	/* led off */
    or_sr(0x10);	/**< CPU OFF */
    P1OUT |= LED;	/* led on */
  }
}

    
    
void
update_shape()
{
  static unsigned char row = screenHeight / 2, col = screenWidth / 2;
  static char blue = 31, green = 0, red = 31;
  static unsigned char step = 0;
  if (switches & SW4) return;
  if (step <= 10) {
    int startCol = col - step;
    int endCol = col + step;
    int width = 1 + endCol - startCol;
    // a color in this BGR encoding is BBBB BGGG GGGR RRRR
    unsigned int color = (blue << 11) | (green << 5) | red;
    squareface(step, color);
    squareface(step, color);
    if (switches & SW3) green = (green + 1) % 64;
    if (switches & SW2) blue = (blue + 2) % 32;
    if (switches & SW1) red = (red - 3) % 32;
    step ++;
  } else {
     clearScreen(COLOR_BLUE);
     step = 0;
  }
}

void squareface(int step, int color)
{
  //The first one is the background for face
  fillRectangle(10, 10, 110, 110, COLOR_ORANGE);
  fillRectangle(20, 20, 35+step, 35+step, color);
  fillRectangle(75, 20, 35+step, 35+step, color);
  fillRectangle(25, 80, 20+step, 30+step, color);
  fillRectangle(85, 80, 20+step, 30+step, color);
  fillRectangle(25, 100, 80+step, 10+step, color);
}

void button1()
{
  if(update)
    {
      update = 0;
      //drawString5x7(10, 100, "Button1", COLOR_RED, COLOR_BLUE);
      update_shape();
      buzzer_set_period(0);
    }
}

void button2()
{
  if(update)
    {
      update = 0;
      drawString5x7(2, 125, "Do you hear the sound", COLOR_GREEN, COLOR_BLUE);
      drawString5x7(2, 130, "?", COLOR_RED, COLOR_BLUE);
      sound();
    }
}

void button3()
{
  if(update)
    {
      update = 0;
      drawString11x16(10, 140, "BIG DOG", COLOR_RED, COLOR_BLUE);
      buzzer_set_period(0);
    }
}

void button4()
{
  if(update)
    {
      clearScreen(COLOR_BLUE);
      buzzer_set_period(0);
    }
}

void defaultBehaviour()
{
  if(update)
    {
      update = 0;
      drawString5x7(10, 160, "Default Behaviour", COLOR_RED, COLOR_BLUE);
    }
}

void buttonSelected()
{
  if (switches & SW1) button1();
  else if (switches & SW2) button2();
  else if (switches & SW3) button3();
  else if (switches & SW4) button4();
  else defaultBehaviour();
}

/* Switch on S2 */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {	      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;		      /* clear pending sw interrupts */
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

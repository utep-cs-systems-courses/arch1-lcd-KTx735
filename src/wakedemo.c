#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include "buzzer.h"
#include "led.h"
#include "sound.h"

// WARNING: LCD DISPLAY USES P1.0.  Do not touch!!! 
#define LED BIT6             /* note that bit zero req'd for display */

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES 15

int switches = 0;
int cycle = 500;
char state = 0;

static char switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);/* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);/* if switch down, sense up */
  return p2val;
}

void switch_init()/* setup switch */

{
  P2REN |= SWITCHES;/* enables resistors for switches */
  P2IE |= SWITCHES;/* enable interrupts from switches */
  P2OUT |= SWITCHES;/* pull-ups for switches */
  P2DIR &= ~SWITCHES;/* set switches' bits for input */
  switch_update_interrupt_sense();
}

int press = 0;
short redrawScreen = 1;

void switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
}

void wdt_c_handler()
{
  static int secCount = 0;
  
  secCount ++;
  if (secCount >= 25) {/* 10/sec */
    secCount = 0;
    redrawScreen = 1;
  }
}

void update_shape();
void draw_stringstuff();
void silence();
void squareface(u_int color);

void main()
{
  P1DIR |= LEDS;/* Green led on when CPU on */
  P1OUT |= LEDS;
  configureClocks();
  lcd_init();
  switch_init();
  buzzer_init();

  enableWDTInterrupts();      /* enable periodic interrupt */
  or_sr(0x8);              /* GIE (enable interrupts) */
  
  clearScreen(COLOR_BLUE);
  while (1) {/* forever */
    if (redrawScreen) {
      redrawScreen = 0;
      update_shape();
    }
    P1OUT &= ~LEDS;/* led off */
    or_sr(0x10);/* CPU OFF */
    P1OUT |= LEDS;/* led on */
  }
}

u_int color0 = COLOR_BLACK;
u_int color1 = COLOR_GREEN;
u_int color2 = COLOR_PINK;
u_int color3 = COLOR_BLUE;
u_int color4 = COLOR_RED;
u_int color5 = COLOR_PURPLE;
u_int move = 0;

void update_shape()
{
  if(switches & SW1){
  squareface(color1);
  squareface(color0);
  squareface(color3);
  squareface(color5);
  squareface(color4);
  }if(switches & SW2){
    //drawString11x16(10, 20, "TEST", COLOR_WHITE, COLOR_BLUE);
    squareface(color1);
    sound();
  }if(switches & SW3){
    silence();
    draw_stringstuff();
  }else{
    squareface(color5);
  }
}

void draw_stringstuff()
{
  drawString11x16(10,20, "STUFF", COLOR_WHITE, COLOR_BLUE);
  drawString11x16(10,40, "STUFF", COLOR_WHITE, COLOR_BLUE);
  drawString11x16(10,60, "STUFF", COLOR_WHITE, COLOR_BLUE);
  drawString11x16(10,80, "STUFF", COLOR_WHITE, COLOR_BLUE);
}

void squareface(u_int color)
{
    fillRectangle(10,10,110,110,COLOR_ORANGE);
    fillRectangle(20,20,35,35,color);
    fillRectangle(75,20,35,35,color);
    fillRectangle(25,80,20,30,color);
    fillRectangle(85,80,20,30,color);
    fillRectangle(25,100,80,10,color);
}

void silence(){
  buzzer_set_period(0);
}

/* Switch on S2 */
void __interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;      /* clear pending sw interrupts */
    switch_interrupt_handler();/* single handler for all switches */
  }
}

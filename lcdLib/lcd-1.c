/** \file lcddemo.c
 *  \brief A simple demo that draws a string and square
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

/** Initializes everything, clears the screen, draws "hello" and a square */
int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  short centerRow = LONG_EDGE_PIXELS/2; // Center coordinate for the rows
  short centerCol = SHORT_EDGE_PIXELS/2; //Center coordinate for the columns

  for(int append = 0; append <= 30; append++)
    {
      for(int c = 30; c >= 0; c--){
	for(int j-0; j <=c; c++){
	  short r = 30-c;
	  drawPixel(centerCol+j, centerRow+(c), COLOR_WHITE);
	}
      }
    }

    
  //Red Triangle (Left edge to right edge)
  /*
  short col = 30;
  short row = 0;
  short startcol = 30;
  short startrow = 30;
  
  for (col = 0; col<30;)
    {
      short row = col;
      short leftEdge = -col;
      short rightEdge = col;
      for(short c = leftEdge; c <= rightEdge; c++)
      drawPixel(startcol+c, startrow+row, COLOR_RED);
      col +=1;
    }
  */
  
  // drawString5x7(20,20, "hello", COLOR_GREEN, COLOR_RED);

  // fillRectangle(30,30, 60, 60, COLOR_ORANGE);
  
}

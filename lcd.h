/*
 *  Module: lcd
 *  Author: M. J. Cree
 *
 *  (C) 2009-2014 The University of Waikato
 *
 *  Routines to interface to the 16x2 text LCD display.
 */

#ifndef _LIBRARY_LCD_H_
#define _LIBRARY_LCD_H_

#include <stdio.h>

/*
 *  lcd_init()
 *
 *  Initialises the LCD for use.  Port D is used for the data lines
 *  of the LCD display.  The control lines are expected to be on
 *  Port C bits 3 to 5 if port is 0 and on Port B bits 3 to 5
 *  if port is 1.
 *
 *  lcd_clear()
 *
 *  Clears the LCD.
 *
 *  lcd_display(text)
 *
 *  Display text on LCD display.
 *
 *  The LCD display has two lines of 16 chars.  The complete message must be
 *  terminated by a string terminator character (i.e. 0).  The end of the
 *  first line should be indicated with a new line character (i.e. '\n') if
 *  there are two lines of text to display or a 0 if there is only one line
 *  of text to display.  No more than 16 characters per line can be
 *  displayed.
 *
 *  lcd_fputc()
 *
 *  The lcd_fputc() function can be used with the stdio library facilities.
 *  At the start of the program you must attach lcd_fputc() to the file
 *  stdout.  Then you can use stdio routines such as putchar() and printf()
 *  and they will print to the LCD.  To attach lcd_fputc() to stdout place
 *  the following at the top of the program:
 *
 *    static FILE mystdout = FDEV_SETUP_STREAM(lcd_fputc, NULL, _FDEV_SETUP_WRITE);
 *
 *  and at the start of main() after calling lcd_init() do the following:
 *
 *    stdout = &mystdout;
 *
 *  and then you can use printf() to print to the LCD.  The following control
 *  characters are implemented in lcd_fputc():
 *
 *    \b -- backspace (deletes char before cursor and moves cursor back one)
 *    \f -- form feed (clears screen and sets cursor to top-left)
 *    \n -- newline (move cursor to start of next line)
 */

extern void lcd_init(int port);
extern void lcd_clear(void);
extern void lcd_display(char *text);
extern int lcd_fputc(char c, FILE *f);

/* For use with lcd_init() */
/* Set port to one of these to put control lines on Port B or Port C. */
#define LCD_CTRL_PORT_B 0x01
#define LCD_CTRL_PORT_C 0x00

/* OR port with LCD_DATA_PORT_D_C if need to split data lines so that
 * bits 2-7 are on Port D 2-7 and bits 0 and 1 are on Port C bits 1 and 2.
 */
#define LCD_DATA_PORT_D 0x00
#define LCD_DATA_PORT_D_C 0x02

#endif

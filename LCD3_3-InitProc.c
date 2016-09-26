/*
 * Program: LCD3_3-InitProc.c
 * Author:  D. O. Corlett
 *
 * Initialisation, and display functions
 * for dot matrix LCD.
 */


#include <util/delay.h>
#include <avr/io.h>
#include <util/delay_basic.h>

	//define LCD control signal macros
#define LCD_RS 0x20
#define LCD_RW 0x10
#define LCD_EN 0x08

int bitmode = 0;

// Delays from 0 us to 65535 us
static inline void delay_us(unsigned int microsecs)
{
	_delay_loop_2(4*microsecs);
}

// Wait for the LCD to be ready to receive data by reading the busy flag.
void LCD_checkBusy()
{
	// Save the state of port c and d registers.
	int PCstate = DDRC;
	int PDstate = DDRD;

	DDRC |= 0x38;		// Set pins 4, 5, and 6 to outputs.

	// Set control signals to read from instruction register.
	PORTC &= ~LCD_EN;
	PORTC &= ~LCD_RS;	// RS = 0, instruction register
	PORTC |= LCD_RW;	// RW = 1, read
	PORTC |= LCD_EN;

	DDRD &= ~(1<<7);	// Set pin 7 on port d to input.

	// Check for busy flag to be 0
	while(PIND & (1<<7))
	{
		PORTC &= ~LCD_EN;
		PORTC |= LCD_EN;
	}
	delay_us(1);		// Very small delay in case LCD isn't quite ready.
	// Restore port c and d register i/o states.
	DDRC = PCstate;
	DDRD = PDstate;
}

// Sends instruction to LCD in either 8 bit mode or 4 bit mode.
void LCD_writeInstruction(int instruction)
{
	// 8 bit mode:
	if (bitmode == 8) {
		// Save the state of port c and d registers:
		int PCstate = DDRC;
		int PDstate = DDRD;
		// Set all pins on port c and d to outputs:
		DDRC = 0xFF;
		DDRD = 0xFF;
		// Send instruction to LCD:
		PORTC &= ~LCD_EN;		// Disable instruction read.
		PORTD = instruction;	// Load the instruction to portD.
		PORTC &= ~LCD_RS;		// RS=0 instruction register.
		PORTC &= ~LCD_RW;		// RW=0 write.
		PORTC |= LCD_EN;		// Enable instruction read
		LCD_checkBusy();		// Wait for LCD to process instruction.
		// Restore port c and d register i/o states.
		DDRC = PCstate;
		DDRD = PDstate;
	}

	// 4 bit mode:
	if (bitmode == 4)
	{
		// Save the state of port c and d registers:
		int PCstate = DDRC;
		int PDstate = DDRD;
		// Set all pins on port c and d to outputs:
		DDRC = 0xFF;
		DDRD = 0xFF;

		PORTC &= ~LCD_RS;					// RS=0 instruction register.
		PORTC &= ~LCD_RW;					// RW=0 write.

		// Send instruction to LCD:
		// Send the 4 MSB of the instruction first:
		PORTC &= ~LCD_EN;					// Disable instruction read.
		PORTD = 0xF0 & instruction;			// Load 4 MSBs of the instruction to portD.
		PORTC |= LCD_EN;					// Enable instruction read
		delay_us(800);
		PORTC &= ~LCD_EN;					// Disable instruction read.
		delay_us(800);						// Wait for LCD to process instruction.
		// Send the 4 LSB of the instruction second:
		PORTC &= ~LCD_EN;					// Disable instruction read.
		PORTD = 0xF0 & (instruction<<4);	// Load 4 LSBs of the instruction to portD.
		PORTC |= LCD_EN;					// Enable instruction read
		delay_us(800);
		PORTC &= ~LCD_EN;					// Disable instruction read.
		LCD_checkBusy();					// Wait for LCD to process instruction.
		// Restore port c and d register i/o states.
		DDRC = PCstate;
		DDRD = PDstate;
	}
}

// Write a character to DDRAM in either 8 bit mode or 4 bit mode.
void LCD_writeToDDRAM(int data, int address)
{
	// Save the state of port c and d registers:
	int PCstate = DDRC;
	int PDstate = DDRD;
	// Set all pins on port c and d to outputs:
	DDRC = 0xFF;
	DDRD = 0xFF;

	// Set DD RAM address
	LCD_writeInstruction(0x80 | address);

	PORTC |= LCD_RS;			// RS=1 data register.
	PORTC &= ~LCD_RW;			// RW=0 write.


	// 8 bit mode:
	if (bitmode == 8)
	{
		// Send data to LCD:
		PORTC &= ~LCD_EN;		// Set Enable = 0.
		PORTD = data;			// Load the data to portD.
		PORTC |= LCD_EN;		// Set Enable = 1.
		LCD_checkBusy();		// Wait for LCD to process instruction.
	}
	// 4 bit mode:
	if (bitmode == 4)
	{
		// Send data to LCD:
		PORTC &= ~LCD_EN;			// Set Enable = 0.
		PORTD = 0xF0 & data;		// Load 4 MSBs of the data to portD.
		PORTC |= LCD_EN;			// Set Enable = 1.
		delay_us(800);
		PORTC &= ~LCD_EN;			// Disable instruction read.
		delay_us(800);				// Wait for LCD to process instruction.

		PORTC &= ~LCD_EN;			// Set Enable = 0.
		PORTD = 0xF0 & (data<<4);	// Load 4 LSBs of the data to portD.
		PORTC |= LCD_EN;			// Set Enable = 1.
		delay_us(800);
		PORTC &= ~LCD_EN;			// Disable instruction read.
		LCD_checkBusy();			// Wait for LCD to process instruction.
	}

	// Restore port c and d register i/o states.
	DDRC = PCstate;
	DDRD = PDstate;
}

// Initialise the LCD for Control on portC and Data on portD.
void my_lcd_init(int bit)
{
		// Set all pins on port c and d to outputs:
	DDRC = 0xFF;
	DDRD = 0xFF;

	_delay_ms(45);					// Wait for 45 ms or more after VDD reaches 4.5 V.
	LCD_writeInstruction(0x30);		// Function Set
	delay_us(4100);					// Wait 4.1 ms or more.

	LCD_writeInstruction(0x30);		// Function Set
	delay_us(100);					// Wait 100 us or more.

	LCD_writeInstruction(0x30);		// Function Set
	delay_us(100);					// Wait 100 us or more.

		// if initialise in 8 bit mode:
	if(bit == 8)
	{
		bitmode = 8;					// Set to 8 bit mode

		LCD_writeInstruction(0x38);		// Function Set Duty: 1/16, Font: 5x7. ExT=40us
		LCD_writeInstruction(0x08);		// Display OFF	 ExT=40us
		LCD_writeInstruction(0x01);		// Display Clear	ExT=1.64ms
		LCD_writeInstruction(0x06);		// Entry Mode Set I/D: move right, S: no shift.	ExT=40us
		LCD_writeInstruction(0x0C);		// Display ON	 ExT=40us
	}
		// if initialise in 4 bit mode:
	if(bit == 4)
	{
		bitmode = 4;					// Set to 4 bit mode

		PORTC &= ~LCD_EN;
		PORTD = 0x20;				//Function Set change to 4bit interface. ExT=40us
		PORTC |= LCD_EN;
		delay_us(600);

		LCD_writeInstruction(0x28);		// Function Set Duty: 1/16, Font: 5x7. ExT=40us
		LCD_writeInstruction(0x08);		// Display OFF	 ExT=40us
		LCD_writeInstruction(0x01);		// Display Clear	ExT=1.64ms
		LCD_writeInstruction(0x06);		// Entry Mode Set I/D: move right, S: no shift.	ExT=40us
		LCD_writeInstruction(0x0F);		// Display ON	 ExT=40us
	}
}

// Clears the LCD.
void my_lcd_clear()
{
	LCD_writeInstruction(0x01);	// Clear LCD instruction.
}

// Displays the argument string of characters on the LCD.
void my_lcd_display(char string[])
{
	my_lcd_clear();		// Clear the LCD before writing to it.
	int address = 0x00;	// Start DD RAM address at 0x00 (beginning of line 1)

		/* Writes each character of string[] to the LCD.
		 * Moves to second line if new line character
		 * is found or if the end of the first line is
		 * full. */
	for(int i = 0; (i < 32) && (string[i]); i++, address++)
	{
		// If end of line
		if(i == 16)
		{
			address = 0x40;	// Line 2 position 0 address
		}
		// If new line character
		if(string[i] == 10)
		{
			address = 0x40;	// Line 2 position 0 address
			i++; 			// increment string index to next character because '/n' dosn't need to be displayed.
		}
		LCD_writeToDDRAM(string[i], address);	// Write the character to the LCD.
	}
}






int main(void)
{
	my_lcd_init(4);			// Initialise LCD, argument: 8 for 8 bit mode, 4 for 4 bit mode.

	char con[80];
	unsigned int num = 0;

	while (1)
    {
			sprintf(con, "%s%c%i", "HELLO", 10, num);	// Create a concatenated string with text string and number.
			my_lcd_display(con);						// Display concatenated string on LCD.
			_delay_ms(1000);							// wait for 0.5 seconds to display the number.
			num++;											// increment the number to be displayed.
    }

    return 0;
}

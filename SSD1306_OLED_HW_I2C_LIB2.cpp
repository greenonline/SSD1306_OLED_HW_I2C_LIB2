/*
** SSD1306_OLED_HW_I2C_LIB2 — a minimalistic adaptation of Neven Boyanov’s SSD1306 library (http://tinusaur.org)
** Author: Ilya Perederiy
** Licence: open source (see LICENSE.txt for details)
** https://bitbucket.org/paraglider/ssd1306_oled_hw_i2c/
**
** Modified by Greenonline 2017
** Enhanced version of SSD1306_OLED_HW_I2C_LIB:
**   Added 32 line display support
**     See config_SSD1306_OLED_HW_I2C_LIB2.h:
**                                           //#define __Using_64_line_OLED__  // This is the default
**                                           #define __Using_32_line_OLED__
**   Fixed inverted line bug which appears in 32 line display (does this bug happen in 64 line displays?)
**     See config_SSD1306_OLED_HW_I2C_LIB2.h:
**                                           #define __Using_inverted_vertical_line_fix__
**   Added updated character set and moved to:
**                                           font5x8.h                  // Original font set
**                                           font5x8_extended.h         // Extended font set ASCII + ° + Ω
**                                           font5x8_extended_custom.h  // Extended font set ASCII + ° + Ω with custom characters
**   Added void SSD1306_OLED_HW_I2C_PRINT_LIB2::D_PRINT_STR(char *s, size_t size)
**   Moved to protected (no longer private):
**                                           D_START_DAT
**                                           D_TX
**                                           D_STOP
**   Added D_DEMO_32() and D_DEMO_64()
**   Modified D_DEMO to use const int height;

This library is designed to control a 128x64 and 128x32 OLED display with an SSD1306 controller over I2C. In essence, it is a minimalistic adaptation of a library written for ATTiny85 and similar micro-controllers. The purpose of this adaptation is to reduce the memory footprint, and add hardware support for I2C communication (supported on ATmega328P, ATmega32U4 and some other micro-controllers). The library can be used with Arduino IDE or in a plain C environment.

The following functions have been implemented in the library:
   - initialize display				D_INIT();
   - clear display					D_CLEAR();
   - turn off (sleep)					D_OFF();
   - turn on (wake up)					D_ON();
   - change brightness (same as contrast)		D_CONTRAST (0-255 or 0x00-0xFF);	
   - set position					D_SETPOS(x coordinate [0-127], character row [0-7]);	// (0,0) corresponds to the upper left corner, 
   - print string (8x6 ascii font)			D_PRINT_STR(“string”);
   - print variable(integers only)			D_PRINT_INT(integer or int variable);
   - draw horizontal line				D_DRAW_HOR(starting x coordinate [0-127], starting y coordinate [0-63], length);
   - draw vertical line				D_DRAW_VERT(starting x coordinate [0-127], starting y coordinate [0-63], length);
   - demonstration mode				D_DEMO();

Note: even though it is possible to specify the exact y coordinate in D_DRAW_HOR, 8 adjacent pixel rows will be rendered (but only one of these rows will light up). As a result any text that was printed in the same group of rows will be overwritten. For example, the following code will result in the line completely erasing the text because pixel rows 0-6 will be rendered dark:

D_SETPOS(0, 1);	
D_PRINT_STR(“some text”);
D_DRAW_HOR(0, 7, 128);


Below are credits from the original SSD1306 library:

  * Copyright (c) 2016 Neven Boyanov, Tinusaur Team. All Rights Reserved.
  * Distributed as open source software under MIT License, see LICENSE.txt file.
  * Please, as a favor, retain the link http://tinusaur.org to The Tinusaur Project.
  * Source code available at: https://bitbucket.org/tinusaur/ssd1306xled

  * NUM2STR - Function to handle the conversion of numeric vales to strings.
  * @created	2014-12-18
  * @author	Neven Boyanov
  * @version	2016-04-17 (last modified)
  * NOTE: This implementation is borrowed from the LCDDDD library.
  * Original source code at: https://bitbucket.org/boyanov/avr/src/default/lcdddd/src/lcdddd/lcdddd.h
*/

#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "SSD1306_OLED_HW_I2C_LIB2.h"

// Font sets
//#include "font5x8.h"                // Original font set
//#include "font5x8_extended.h"       // Extended font set ASCII + ° + Ω
#include "font5x8_extended_custom.h"  // Extended font set ASCII + ° + Ω with custom characters



// Display initialization sequence
const uint8_t init_sequence [] PROGMEM = {
	0xAE,			// Display OFF (sleep mode)
	0x20, 0b00,		// Set Memory Addressing Mode
					// 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
					// 10=Page Addressing Mode (RESET); 11=Invalid
	0xB0,			// Set Page Start Address for Page Addressing Mode, 0-7
	0xC8,			// Set COM Output Scan Direction
	0x00,			// ---set low column address
	0x10,			// ---set high column address
	0x40,			// ---set start line address
	0x81, 0x00,		// Set contrast control register
	0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped. 
	0xA6,			// Set display mode. A6=Normal; A7=Inverse
#if defined (__Using_32_line_OLED__)
	0xA8, 0x1F,		// Set multiplex ratio(1 to 32)
#elif defined (__Using_64_line_OLED__)
	0xDA, 0x12,		// Set com pins hardware configuration		
#else
	0xA8, 0x3F,		// Set multiplex ratio(1 to 64)
#endif
	0xA4,			// Output RAM to Display
					// 0xA4=Output follows RAM content; 0xA5,Output ignores RAM content
	0xD3, 0x00,		// Set display offset. 00 = no offset
	0xD5,			// --set display clock divide ratio/oscillator frequency
	0xF0,			// --set divide ratio
	0xD9, 0x22,		// Set pre-charge period
#if defined (__Using_32_line_OLED__)
	0xDA, 0x02,		// Set com pins hardware configuration		
#elif defined (__Using_64_line_OLED__)
	0xDA, 0x12,		// Set com pins hardware configuration		
#else
	0xDA, 0x12,		// Set com pins hardware configuration		
#endif
	0xDB,			// --set vcomh
	0x20,			// 0x20,0.77xVcc
	0x8D, 0x14,		// Set DC-DC enable
	0xAF			// Display ON in normal mode
};


/*
// An alternative initialization sequence
const uint8_t init_sequence [] PROGMEM = {	// Initialization Sequence
    0xAE,			// Display OFF (sleep mode)
    0xA8, 0x3F,     // Set mux ratio tp select max number of rows - 64
    0xD3, 0x00,		// Set display offset. 00 = no offset
    0xA1,			// Set Segment Re-map. A0=address mapped; A1=address 127 mapped.
	0xC8,			// Set COM Output Scan Direction
    0xDA, 0x12,     // Default - alternate COM pin map
    0x81, 0x00,		// Set contrast control register
	0xA4,			// Set display to enable rendering from GDDRAM (Graphic Display Data RAM)
    0xA6,			// Set display mode. A6=Normal; A7=Inverse
	0xD5, 0x80,     // Default oscillator clock (alt 0xF0)
    0x8D, 0x14,     // Enable the charge pump
    0xD9, 0x22,		// Set pre-charge period
	0xDB, 0x30,     // Set the V_COMH deselect volatage to max (alt 0x20 for 0.77xVcc)
    0x20, 0b00,		// Set Memory Addressing Mode
                    // 00=Horizontal Addressing Mode; 01=Vertical Addressing Mode;
                    // 10=Page Addressing Mode (RESET); 11=Invalid
    0xAF			// Display ON in normal mode
};
*/

char ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS + 1];

//constructors

SSD1306_OLED_HW_I2C_LIB2::SSD1306_OLED_HW_I2C_LIB2(void)
{
  // SCL bit rate = CLK / (16 + 2*TWBR*[TWSR prescaler])
  TWSR = 0x00;    // I2C prescaler 1
  TWBR = 2;       // I2C divider 2
}


void SSD1306_OLED_HW_I2C_LIB2::CLK_DIV_1(void) {          // Set clock divider to 1 (fast operation)
    cli();
    CLKPR = (1<<CLKPCE);
    CLKPR = 0x00;                          // div 1
    sei();
}

void SSD1306_OLED_HW_I2C_LIB2::CLK_DIV_8(void) {          // Set clock divider to 8 (slow operation)
    cli();
    CLKPR = (1<<CLKPCE);
    CLKPR = (1<<CLKPS1)|(1<<CLKPS0);        // div 8
    sei();
}

uint8_t SSD1306_OLED_HW_I2C_LIB2::usint2decascii(uint16_t num, char *buffer)      // convert integer to string
{
// @author	Neven Boyanov
// NOTE: This implementation is borrowed from the LCDDDD library.
// Original source code at: https://bitbucket.org/boyanov/avr/src/default/lcdddd/src/lcdddd/lcdddd.h

	const unsigned short powers[] = { 10000u, 1000u, 100u, 10u, 1u }; // The "const unsigned short" combination gives shortest code.
	char digit; // "digit" is stored in a char array, so it should be of type char.
	uint8_t digits = USINT2DECASCII_MAX_DIGITS - 1;
	for (uint8_t pos = 0; pos < 5; pos++) // "pos" is index in array, so should be of type int.
	{
		digit = 0;
		while (num >= powers[pos])
		{
			digit++;
			num -= powers[pos];
		}
		// ---- CHOOSE (1), (2) or (3) ----
		// CHOICE (1) Fixed width, zero padded result.
		/*
		//buffer[pos] = digit + '0';	// Convert to ASCII
		*/
		// CHOICE (2) Fixed width, zero padded result, digits offset.
		/*
		buffer[pos] = digit + '0';	// Convert to ASCII
		// Note: Determines the offset of the first significant digit.
		if (digits == -1 && digit != 0) digits = pos;
		// Note: Could be used for variable width, not padded, left aligned result.
		*/
		// CHOICE (3) Fixed width, space (or anything else) padded result, digits offset.
		// Note: Determines the offset of the first significant digit.
		// Note: Could be used for variable width, not padded, left aligned result.
		if (digits == USINT2DECASCII_MAX_DIGITS - 1)
		{
			if (digit == 0)
			{
				if (pos < USINT2DECASCII_MAX_DIGITS - 1)	// Check position, so single "0" will be handled properly.
					digit = -16;	// Use: "-16" for space (' '), "-3" for dash/minus ('-'), "0" for zero ('0'), etc. ...
			}
			else
			{
				digits = pos;
			}
		}
		buffer[pos] = digit + '0';	// Convert to ASCII
	}
	// NOTE: The resulting ascii text should not be terminated with '\0' here.
	//       The provided buffer maybe part of a larger text in both directions.
	return digits;
}


void SSD1306_OLED_HW_I2C_LIB2::D_ERROR(void) {                            // I2C comm error handler. Blinks ERROR_PIN in PORTD
    for (uint8_t i = 0; i < 10; i++) {
        PORTD ^= 1 << ERROR_PIN;
        _delay_ms(300);
    }
}


void SSD1306_OLED_HW_I2C_LIB2::D_START_CMD(void) {                        // Start I2C and tell the display to await commands
    cli();                                      // disable interrupts for the time being
    //CLK_DIV_1();                                // increase clock speed to max
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);     // START I2C
    while (!(TWCR&(1<<TWINT)));                 // wait
    if ((TWSR & 0xF8) != 0x08) D_ERROR();       // success or error


    TWDR = SLA_W;                               // slave address
    TWCR = (1<<TWINT) |(1<<TWEN);               // transmit
    while (!(TWCR & (1<<TWINT)));               // wait for transmission
    if ((TWSR & 0xF8) != 0x18) D_ERROR();       // success or error

    D_TX(0x00);                                 // prep command stream: C0=0 D/C#=0, followed by 6 zeros (datasheet 8.1.5.2)
}

void SSD1306_OLED_HW_I2C_LIB2::D_START_DAT(void) {                        // Start I2C and tell the display to await data (pixels)
    cli();                                      // disable interrupts for the time being
    //CLK_DIV_1();                                // increase clock speed to max
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);     // START I2C
    while (!(TWCR&(1<<TWINT)));                 // wait
    if ((TWSR & 0xF8) != 0x08) D_ERROR();       // success or error

    TWDR = SLA_W;                               // slave address
    TWCR = (1<<TWINT) |(1<<TWEN);               // transmit
    while (!(TWCR & (1<<TWINT)));               // wait for transmission
    if ((TWSR & 0xF8) != 0x18) D_ERROR();       // success or error

    D_TX(0x40);                                 // prep for data stream: C0 = 0 D/C#=1, followed by 6 zeros (datasheet 8.1.5.2)
}

void SSD1306_OLED_HW_I2C_LIB2::D_TX(uint8_t DATA) {                       // transmit 1 byte
    TWDR = DATA;                                // data to transmit
    TWCR = (1<<TWINT) |(1<<TWEN);               // transmit
    while (!(TWCR & (1<<TWINT)));               // wait for transmission
    if ((TWSR & 0xF8) != 0x28) D_ERROR();       // success or error
}

void SSD1306_OLED_HW_I2C_LIB2::D_STOP (void) {                            // Stop I2C communication
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);     // stop
    //CLK_DIV_8();                                // decrease CLK speed
    sei();                                      // re-enable interrupts
}


// 

void SSD1306_OLED_HW_I2C_LIB2::D_INIT(void) {                             // Initialize display
    D_START_CMD();
    for (uint8_t i = 0; i < sizeof (init_sequence); i++) {
        D_TX(pgm_read_byte(&init_sequence[i]));}            // read init sequence from progmem
    D_TX(OLED_CMD_SET_COLUMN_RANGE);
    D_TX(0x00);
    D_TX(0x7F);
    D_TX(OLED_CMD_SET_PAGE_RANGE);
    D_TX(0);
    D_TX(0x07);
    D_STOP();
}


void SSD1306_OLED_HW_I2C_LIB2::D_SETPOS(uint8_t x, uint8_t y) {           // Set cursor position
    D_START_CMD();
	D_TX(0xB0 + y);
	D_TX(((x & 0xF0) >> 4) | 0x10);
    D_TX((x & 0x0f));
	D_STOP();
}

void SSD1306_OLED_HW_I2C_LIB2::D_CLEAR(void) {                            // clear display
    D_SETPOS(0, 0);
    D_START_DAT();
    for (uint16_t i = 0; i < 128 * 8; i++) {
        D_TX(0);
    }
    D_STOP();

}

void SSD1306_OLED_HW_I2C_LIB2::D_ON(void) {                               // turn on display (wake up)
    D_START_CMD();
    D_TX(0xAF);
    D_STOP();
}

void SSD1306_OLED_HW_I2C_LIB2::D_OFF(void) {                              // turn off display (sleep)
    D_START_CMD();
    D_TX(0xAE);
    D_STOP();
}

void SSD1306_OLED_HW_I2C_LIB2::D_CONTRAST (uint8_t contrast) {           // change contrast (brightness)
    D_START_CMD();
    D_TX(0x81);                                // Set contrast control register
    D_TX(contrast);                            // contrast 0-255
    D_STOP();
}

// Draw a horizontal line
// Note: even though the line is 1px thick, it will affect 8 pixel rows
void SSD1306_OLED_HW_I2C_LIB2::D_DRAW_HOR(uint8_t xpos, uint8_t ypos, uint8_t length) {
    uint8_t ypage = ypos / 8;                       // determine page (8 vertical pixels) from pixel position
    uint8_t dot_byte = 1 << (ypos % 8);             // create a byte with a dot at the specified position within the page
    D_SETPOS(xpos, ypage);
    D_START_DAT();
    for (uint8_t i = 0; i < length; i++) {
        D_TX(dot_byte);
    }
	D_STOP();
}

// Draw a vertical line
void SSD1306_OLED_HW_I2C_LIB2::D_DRAW_VERT(uint8_t xpos, uint8_t ypos, uint8_t length) {
    uint8_t ypage_start = ypos / 8;                         // determine starting page from pixel position
    uint8_t ypage_end = (ypos + length) / 8;                // determine last page from pixel position
    uint8_t ypages_span = ypage_end - ypage_start;          // how many pages does the line span?

    uint8_t dot_byte_start = 0xFF << (ypos % 8);            // create a byte with dots to represent first page (a column of 8 pixels)
#if defined (__Using_inverted_vertical_line_fix__)
    uint8_t dot_byte_end = 0xFF >> (8 - (ypos+length) % 8);     // create a byte with dot to represent last page
// Alternative fix
//    uint8_t dot_byte_end = 0xFF << ((ypos+length) % 8);     // create a byte with dot to represent last page
//    dot_byte_end = ~dot_byte_end;     // create a byte with dot to represent last page
#else
    uint8_t dot_byte_end = 0xFF >> ((ypos+length) % 8);     // create a byte with dot to represent last page
#endif
    
    if (ypages_span == 0) {                                 // line is 8 pixels or shorter
        D_SETPOS(xpos, ypage_start);
        D_START_DAT();
        D_TX(dot_byte_start&dot_byte_end);
        D_STOP();
    }
    else {                                                  // line is >8 pixels long
        D_SETPOS(xpos, ypage_start);
        D_START_DAT();
        D_TX(dot_byte_start);                               // draw first 8 pixels (first page)
        D_STOP();
        
        if (ypages_span > 1) {                              // if the line >16 pixels long, draw the intermediate pages
            for (uint8_t i = ypage_start + 1; i < ypage_end; i++) {
                D_SETPOS(xpos, i);
                D_START_DAT();
                D_TX(0xFF);
                D_STOP();
            }
        }
        D_SETPOS(xpos, ypage_end);
        D_START_DAT();
        D_TX(dot_byte_end);                                 // draw the last 8 pixels (last page)
        D_STOP();
    }
}


void SSD1306_OLED_HW_I2C_LIB2::D_PRINT_CHAR(char ch) {                                // print 1 character
	uint8_t c = ch - 32;
    D_START_DAT();
    D_TX(0x00);                                             // character leading 1 px space
	for (uint8_t i = 0; i < 5; i++)
    {
		D_TX(pgm_read_byte(&D_FONT6x8[c * 5 + i]));
	}
	D_STOP();
}

void SSD1306_OLED_HW_I2C_LIB2::D_PRINT_STR(char *s) {                                 // print string (char array)
	while (*s) {
		D_PRINT_CHAR(*s++);
	}
}

// print string (char array)
void SSD1306_OLED_HW_I2C_LIB2::D_PRINT_STR(char *s, size_t size) {                                 
  for (int n=0; n < size; n++) {
    D_PRINT_CHAR(s[n]);
  }
}


void SSD1306_OLED_HW_I2C_LIB2::D_PRINT_INT(uint16_t num) {                            // print integer variable
	ssd1306_numdec_buffer[USINT2DECASCII_MAX_DIGITS] = '\0';   // Terminate the string.
	uint8_t digits = usint2decascii(num, ssd1306_numdec_buffer);
	D_PRINT_STR(ssd1306_numdec_buffer + digits);
}

// These are used for D_DEMO/D_DEMO_32/D_DEMO_64
#if defined (__Using_32_line_OLED__)
const int height = 32;
#elif defined (__Using_64_line_OLED__)
const int height = 64;
#else
const int height = 64;
#endif

void SSD1306_OLED_HW_I2C_LIB2::D_DEMO(void) {                                         // display demonstration 
        D_CLEAR();                                          // clear display
        D_DRAW_HOR(0, 0, 127);                              // top horiz line (start x, start y, length) - this doesn't print top line
        D_DRAW_HOR(0, height-1, 127);                             // bottom horiz line (start x, start y, length)
        D_DRAW_VERT(0, 0, height);                              // left vert line (start x, start y, length)
        D_DRAW_VERT(127, 0, height);                            // right vert line (start x, start y, length)
        D_SETPOS(25,1);                                     // set cursor position
        D_PRINT_STR("DEMONSTRATION");                       // print message
        D_SETPOS(6,3);
        D_PRINT_STR("The display will be");
        D_SETPOS(34,4);
        D_PRINT_STR("turned off");
        D_SETPOS(30,5);
        D_PRINT_STR("temporarily");
        _delay_ms(2000);
        D_OFF();                                            // turn off display (conserve power)
        _delay_ms(500);
        D_CLEAR();
        D_ON();                                             // turn on display
        _delay_ms(500);

        D_SETPOS(2,3);
        D_PRINT_STR("   Counter = ");
        for (uint16_t i = 800; i>0; i--) {
            D_SETPOS(2+13*6,3);
            D_PRINT_INT(i);                                 // print counter variable
        }
    
        D_CLEAR();
        D_SETPOS(18,4);
        D_PRINT_STR("LOWEST CONTRAST");
        _delay_ms(1000);
        D_CONTRAST(0xFF);                                   // change contrast (0-255 or 0x00-0xFF)
        D_SETPOS(14,4);
        D_PRINT_STR("HIGHEST CONTRAST");
        _delay_ms(1000);
        D_CONTRAST(0x00);
}

void SSD1306_OLED_HW_I2C_LIB2::D_DEMO_32(void) {                                         // display demonstration 
        D_CLEAR();                                          // clear display
        D_DRAW_HOR(0, 0, 127);                              // top horiz line (start x, start y, length)
        D_DRAW_HOR(0, 31, 127);                             // bottom horiz line (start x, start y, length)
        D_DRAW_VERT(0, 0, 32);                              // left vert line (start x, start y, length)
        D_DRAW_VERT(127, 0, 32);                            // right vert line (start x, start y, length)
        D_SETPOS(25,1);                                     // set cursor position
        D_PRINT_STR("DEMONSTRATION");                       // print message
        D_SETPOS(6,3);
        D_PRINT_STR("The display will be");
        D_SETPOS(34,4);
        D_PRINT_STR("turned off");
        D_SETPOS(30,5);
        D_PRINT_STR("temporarily");
        _delay_ms(2000);
        D_OFF();                                            // turn off display (conserve power)
        _delay_ms(500);
        D_CLEAR();
        D_ON();                                             // turn on display
        _delay_ms(500);

        D_SETPOS(2,3);
        D_PRINT_STR("   Counter = ");
        for (uint16_t i = 800; i>0; i--) {
            D_SETPOS(2+13*6,3);
            D_PRINT_INT(i);                                 // print counter variable
        }
    
        D_CLEAR();
        D_SETPOS(18,4);
        D_PRINT_STR("LOWEST CONTRAST");
        _delay_ms(1000);
        D_CONTRAST(0xFF);                                   // change contrast (0-255 or 0x00-0xFF)
        D_SETPOS(14,4);
        D_PRINT_STR("HIGHEST CONTRAST");
        _delay_ms(1000);
        D_CONTRAST(0x00);
}

void SSD1306_OLED_HW_I2C_LIB2::D_DEMO_64(void) {                                         // display demonstration 
        D_CLEAR();                                          // clear display
        D_DRAW_HOR(0, 0, 127);                              // top horiz line (start x, start y, length)
        D_DRAW_HOR(0, 63, 127);                             // bottom horiz line (start x, start y, length)
        D_DRAW_VERT(0, 0, 64);                              // left vert line (start x, start y, length)
        D_DRAW_VERT(127, 0, 64);                            // right vert line (start x, start y, length)
        D_SETPOS(25,1);                                     // set cursor position
        D_PRINT_STR("DEMONSTRATION");                       // print message
        D_SETPOS(6,3);
        D_PRINT_STR("The display will be");
        D_SETPOS(34,4);
        D_PRINT_STR("turned off");
        D_SETPOS(30,5);
        D_PRINT_STR("temporarily");
        _delay_ms(2000);
        D_OFF();                                            // turn off display (conserve power)
        _delay_ms(500);
        D_CLEAR();
        D_ON();                                             // turn on display
        _delay_ms(500);

        D_SETPOS(2,3);
        D_PRINT_STR("   Counter = ");
        for (uint16_t i = 800; i>0; i--) {
            D_SETPOS(2+13*6,3);
            D_PRINT_INT(i);                                 // print counter variable
        }
    
        D_CLEAR();
        D_SETPOS(18,4);
        D_PRINT_STR("LOWEST CONTRAST");
        _delay_ms(1000);
        D_CONTRAST(0xFF);                                   // change contrast (0-255 or 0x00-0xFF)
        D_SETPOS(14,4);
        D_PRINT_STR("HIGHEST CONTRAST");
        _delay_ms(1000);
        D_CONTRAST(0x00);
}



#include "pcd8544.h"

void LCDWrite(uint8_t dc, uint8_t data) {
	// Check if write is command or data
	if (dc)
		LCD_DC_HI;    // Data
	else
		LCD_DC_LO;    // Command
	
	// Signal LCD to start receiving byte
	LCD_SCE_LO;
	for (uint8_t i = 0; i < 8; i++) {
		// Determine if bit to send is 1 or 0
		if (data&(1<<7))
			LCD_DN_HI;
		else
			LCD_DN_LO;
		
		// Oscillate clock for next bit
		LCD_SCLK_LO;
		LCD_SCLK_HI;
		
		// Shift to next bit
		data <<= 1;
	}
	
	// Signal LCD to end receiving
	LCD_SCE_HI;
	LCD_DN_LO;
}

void gotoXY(unsigned char x, unsigned char y) {
	LCDWrite(0, 0x80 | x);  
	LCDWrite(0, 0x40 | y);  
}

void LCDChar(unsigned char c) {
	// Signal to start write
	LCDWrite(LCD_D, 0x00);
	
	// Send each byte LCDWrite(LCD_D, pgm_read_byte(ascii_table+(c-0x20)*5+i));
	for (uint8_t i = 0; i < 5; i++)
		LCDWrite(LCD_D, ascii_table[c - 0x20][i]);
	
	// Signal to end write
	LCDWrite(LCD_D, 0x00);
}
// Not working properly
void LCDSprite(unsigned char* str) {
	// Signal to start write
	LCDWrite(LCD_D, 0x00);
	
	// Send each byte
	for (uint8_t i = 0; i < 8; i++) // i < 5 original
		LCDWrite(LCD_D, (str[i]));
	
	// Signal to end write
	LCDWrite(LCD_D, 0x00);
}

void LCDStr(const char* str) {
	while (pgm_read_byte(str)) {	
		// Iterate through each char to send
		LCDChar(pgm_read_byte(str));
		str++;
	}
}



void LCDClear(void) {
	// Clear all pixels on LCD
	for (int i = 0; i < LCD_WIDTH*LCD_HEIGHT/8; i++)
		LCDWrite(LCD_D, 0x00);
}
void LCDInit(void) {
	
	// initialize the LCD pins
	LCD_SCE_HI;
	LCD_RST_LO;
	LCD_RST_HI;
	
	// Set of commands
	LCDWrite(LCD_C, 0x21);  // extended instruction set is coming
	LCDWrite(LCD_C, 0xA0);	// Set CONTRAST: set Vop to 3.3V
	LCDWrite(LCD_C, 0x04);	// set the temperature coefficient
	LCDWrite(LCD_C, 0x15);	// set the bias system
	// active mode, horizontal addressing, basic instruction set
	LCDWrite(LCD_C, 0x20);
	LCDWrite(LCD_C, 0x0C);  // normal mode
	LCDClear();             // clear the LCD
}

//This takes a large array of bits and sends them to the LCD
void LCDBitmap(const unsigned char my_array[]) {
	for (unsigned short index = 0 ; index < (LCD_WIDTH * LCD_HEIGHT / 8) ; index++)
		LCDWrite(LCD_D, my_array[index]);
}

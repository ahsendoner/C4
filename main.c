/*
 * MCU_bringup_stewart.c
 *
 * Created: 2021-08-25 2:14:48 PM
 * Author : C4
 */ 


#define F_CPU 8000000
//#define F_CPU		8000000UL // Clock speed

#include <avr/io.h>
#include <util/delay.h> //for delay function
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "./twi.h"
#include "./si5351.h"

#include "i2c.h"
#include "screen_cmds.h"


void screen_init(void)
{
	// TODO: Initialize screen
	I2Csendcmd(SCREEN_ADDR, COMMAND_8BIT_4LINES_NORMAL_RE1_IS0);
	I2Csendcmd(SCREEN_ADDR, COMMAND_NW);
	I2Csendcmd(SCREEN_ADDR, COMMAND_SEGMENT_BOTTOM_VIEW);
	I2Csendcmd(SCREEN_ADDR, COMMAND_BS1_1);
	I2Csendcmd(SCREEN_ADDR, COMMAND_8BIT_4LINES_RE0_IS1);
	I2Csendcmd(SCREEN_ADDR, COMMAND_BS0_1);
	I2Csendcmd(SCREEN_ADDR, COMMAND_FOLLOWER_CONTROL);
	I2Csendcmd(SCREEN_ADDR, COMMAND_POWER_BOOSTER_CONTRAST);
	I2Csendcmd(SCREEN_ADDR, COMMAND_SET_CONTRAST_1010);
	I2Csendcmd(SCREEN_ADDR, COMMAND_8BIT_4LINES_RE0_IS0);
	I2Csendcmd(SCREEN_ADDR, COMMAND_DISPLAY_ON_CURSOR_ON_BLINK_ON);
	
 }

void screen_write_string(char string_to_write[])
{
	int letter=0;
	
	I2Csendcmd(SCREEN_ADDR, COMMAND_CLEAR_DISPLAY);
	I2Csendcmd(SCREEN_ADDR, COMMAND_SET_CURSOR_LINE_1);
	int current_line = COMMAND_SET_CURSOR_LINE_1;
	
	while(string_to_write[letter]!='\0')
	{
		if ((letter != 0) && (letter % LINE_LENGTH == 0))
		{
			if (current_line == COMMAND_SET_CURSOR_LINE_4){
				// We've gone past the end of the screen, go back to top
				current_line = COMMAND_SET_CURSOR_LINE_1;
				// Clear the screen 
				I2Csendcmd(SCREEN_ADDR, COMMAND_CLEAR_DISPLAY);
			}
			else {
				current_line = current_line+0x20;
			}
			// We've gone past the end of the line, go to the next one
			I2Csendcmd(SCREEN_ADDR, current_line); 
		}
		
		I2Csenddatum(SCREEN_ADDR, string_to_write[letter]);
		letter++;
	}
}


int main(void)
{
	uint32_t FA = 8000000;
	bool enabled =true;
	
	// Set CLK to 8 MHz
	CLKPR = 1<<CLKPCE;
	CLKPR = 0;
	
	DDRD = 0xff; //PortD as output (only need PD6 for display)
	DDRA = 0x00;
	const int STR_LEN = 40;
	const float VREF = 3.3; // Measure this with a voltmeter
	volatile char string_to_write[STR_LEN];
	uint32_t display_freq = 8000;
	volatile char str[40];
	volatile int enter = 0;
    volatile int mode = 0; // starts from transmit mode
	
	//set PIN4 as input
  DDRA = DDRA & ~(1 << PA0); //+1
  DDRA = DDRA & ~(1 << PA1); //+10
  DDRA = DDRA & ~(1 << PA2); //+100
  DDRA = DDRA & ~(1 << PA3); //+1000
  DDRA = DDRA & ~(1 << PA4); // enter
  DDRA = DDRA & ~(1 << PA7); // txen
  DDRD = DDRD | (1 << PD7); //txen supplied to PD7
	
	
	_delay_ms(5);
	PORTD = PORTD & (0<<PD6); // turn off
	_delay_ms(200);
	PORTD = PORTD | (1<<PD6); // turn on display
	_delay_ms(5);
	
	//Set up I2C
	I2Cinit(); // Done
	
	//Initialize display
	screen_init(); // TODO
	
	strncpy(string_to_write,"Hello TEAM C4",STR_LEN);
	screen_write_string(string_to_write);
	/*
	while (1)
	{
		_delay_ms(100);
	}
	//Write some data to get started
	//strncpy(string_to_write,"hello world",STR_LEN);
	*/
	twi_init();
	si5351_init();
	setup_PLL(SI5351_PLL_A, 28, 0, 1);
	set_LO_freq(FA);
	enable_clocks(enabled);
	I2Cinit();
	int lcdon = 1;
	while (1)
	{	
		
		
		volatile int increment1 = (PINA & (1 << PINA0))>>PINA0;
		volatile int increment10 = (PINA & (1 << PINA1))>>PINA1;
		volatile int increment100 = (PINA & (1 << PINA2))>>PINA2;
		volatile int increment1000 = (PINA & (1 << PINA3))>>PINA3;
		volatile int enterpressed = (PINA & (1 << PINA4))>>PINA4;
		volatile int modechange = (PINA & (1 << PINA7))>>PINA7;
		
		_delay_ms(10);
		//if(!(PINA & (1 << PINA0))>>PINA0||!(PINA & (1 << PINA1))>>PINA1||!(PINA & (1 << PINA2))>>PINA2||!(PINA & (1 << PINA3))>>PINA3){
		if (!increment1) {
	
			if(display_freq%10 == 9){
				display_freq = display_freq-9;
				}else{
				display_freq++;
			}
			//goto label;
			//sprintf(str, "PIN 0 is pressed", display_freq);
			sprintf(str, "%d", display_freq);
			strcpy(string_to_write, str);
			screen_write_string(string_to_write);
			_delay_ms(100); // sit idle
			//if pinA4 is high, right it back to how it was so it can read pin 3
		}
		if (!increment10) {
			if((display_freq/10)%10 == 9){
				display_freq = display_freq - 90;
				}else{
				display_freq = display_freq+10;
			}
			//sprintf(str, "PIN 1 is pressed", display_freq);
			
			sprintf(str, "%d", display_freq);
			strcpy(string_to_write, str);
			screen_write_string(string_to_write);
			//goto label;
			_delay_ms(100); // sit idle
			//if pinA4 is high, right it back to how it was so it can read pin 3
		}
		
		
		if (!increment100) {
			if((display_freq/100)%10 == 9){
				display_freq = display_freq - 900;
				}else{
				display_freq = display_freq+100;
			}
			//goto label;
			//sprintf(str, "PIN2 is released", display_freq);
			sprintf(str, "%d", display_freq);
			strcpy(string_to_write, str);
			screen_write_string(string_to_write);
			_delay_ms(100); // sit idle
			//if pinA4 is high, right it back to how it was so it can read pin 3
		}
		if (!increment1000) {
			if(display_freq > 16000){
				display_freq = display_freq - 8000;
				}else{
				display_freq = display_freq+1000;
			}
			//goto label;
			sprintf(str, "%d", display_freq);
			strcpy(string_to_write, str);
			screen_write_string(string_to_write);
			_delay_ms(100); // sit idle
			//if pinA4 is high, right it back to how it was so it can read pin 3
		}
		if (!enterpressed) {
			enter = 1;
			strncpy(string_to_write,"Enter is pressed",STR_LEN);
			screen_write_string(string_to_write);
			_delay_ms(100); // sit idle
			if(lcdon == 1){
			PORTD = PORTD & (0<<PD6); // turn off
			_delay_ms(50);
			uint32_t inputfreq = display_freq*1000;
			set_LO_freq(inputfreq);
			lcdon = 0;
			}
			else if (lcdon == 0){
			PORTD = PORTD | (1<<PD6); // turn on
			_delay_ms(50);
			lcdon = 1;	
			screen_init(); // TODO
			}
			
			//if pinA4 is high, right it back to how it was so it can read pin 3
			//goto idle;
		}
		
		
        if(modechange){
			//strncpy(string_to_write,"RX Mode",STR_LEN);
			//screen_write_string(string_to_write);
            if(mode == 0){
                mode = 1;
                PORTD = PORTD | (1 << 7); //PIN 4 of PD7 is high
                strncpy(string_to_write,"RX Mode",STR_LEN);
                screen_write_string(string_to_write);
				_delay_ms(100);
				//goto idle;
            }else{
                mode = 0;
                PORTD = PORTD & ~(1 << 7);
                strncpy(string_to_write,"TX Mode",STR_LEN);
                screen_write_string(string_to_write);
				_delay_ms(100);
				//goto idle;
            }
            
        }
		//}
		/*
		label:
			sprintf(str, "Frequency: %dM Hz", display_freq);
			strcpy(string_to_write, str);
			screen_write_string(string_to_write);
		idle:
			_delay_ms(10);
			*/
		//screen_write_string(string_to_write);
	}
}

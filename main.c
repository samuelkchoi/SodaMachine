/* 
	Samuel Choi
	*EE/CS 120B Custom Lab Project
	*Soda Machine
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <timer.h>
#include "pcd8544.c"

unsigned short my_adc;

unsigned char output_to_soda, soda_flag; //Variables used 

enum states {init, menu, soda_toggle, soda_toggle_release, flag_check, dispense} state; //Enumerates each state

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
}

void soda_tick()
{
	//soda_flag = 0 means Sprite
	//soda_flag = 1 means Coke
	//soda_flag = 2 means Dr. Pepper
	my_adc = ADC;
	output_to_soda = ~PINC & 0x01;
	switch(state)
	{
		case init:
		LCDInit();
		LCDClear();
		LCDBitmap(Menu);
		state = menu;
		break;
		
		case menu:
		if(my_adc < 350)
		{
			soda_flag = 0;
			state = soda_toggle;
		}
		else if(my_adc > 700)
		{
			soda_flag = 2;
			state = soda_toggle;
		}
		else
		{
			state = menu;
		}
		break;
		
		case soda_toggle:
		if(soda_flag == 0)
		{
			LCDBitmap(Sprite);
			state = soda_toggle_release;
		}
		else if(soda_flag == 1)
		{
			LCDBitmap(Coke);
			state = soda_toggle_release;
		}
		else if(soda_flag == 2)
		{
			LCDBitmap(DrPepper);
			state = soda_toggle_release;
		}
		break;
		
		case soda_toggle_release:
		if(my_adc < 350 || my_adc > 700)
		{
			state = soda_toggle_release;
		}
		else
		{
			state = dispense;
		}
		break;
		
		case dispense:
		if(my_adc < 350 || my_adc > 700)
		{
			state = flag_check;
		}
		else if(output_to_soda && soda_flag == 0)
		{
			PORTD = 0x01;
			state = dispense;
		}
		else if(output_to_soda && soda_flag == 1)
		{
			PORTD = 0x02;
			state = dispense;
		}
		else if(output_to_soda && soda_flag == 2)
		{
			PORTD = 0x04;
			state = dispense;
		}
		else
		{
			PORTD = 0;
			state = dispense;
		}
		break;
		
		case flag_check:
		if(my_adc < 300 && soda_flag == 0)
		{
			soda_flag = 1;
			state = soda_toggle;
		}
		else if(my_adc < 300 && soda_flag == 1)
		{
			soda_flag = 2;
			state = soda_toggle;
		}
		else if(my_adc < 300 && soda_flag == 2)
		{
			soda_flag = 0;
			state = soda_toggle;
		}
		else if(my_adc > 700 && soda_flag == 0)
		{
			soda_flag = 2;
			state = soda_toggle;
		}
		else if(my_adc > 700 && soda_flag == 1)
		{
			soda_flag = 0;
			state = soda_toggle;
		}
		else if(my_adc > 700 && soda_flag == 2)
		{
			soda_flag = 1;
			state = soda_toggle;
		}
		else
		{
			state = flag_check;
		}
		break;
		
		default:
		state = init;
		break;
	}
}

int main(void)
{
    DDRA = 0x00; PORTA = 0xFF; //input
	DDRB = 0xFF; PORTB = 0x00; //output
	DDRC = 0x00; PORTC = 0xFF; //input
	DDRD = 0xFF; PORTD = 0x00; //output
	LCDInit();
	TimerSet(30);
	TimerOn();
	ADC_init();
	while(1)
    {

		soda_tick();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}

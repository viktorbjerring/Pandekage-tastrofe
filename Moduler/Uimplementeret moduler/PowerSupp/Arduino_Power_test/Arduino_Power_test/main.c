/*
 * Arduino_Power_test.c
 *
 * Created: 04/04/2020 10.57.48
 * Author : Mikkel
 */ 

#define F_CPU		16000000

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD |= 0xfc;
	DDRB = 0x3f;
	DDRC = 0x3f;
	
	PORTD |= 0xfc;
	PORTB = 0x3f;
	PORTC = 0x3f;
	/* Replace with your application code */
    while (1) 
    {
		PORTB = 0x00;
		_delay_ms(1000);
		PORTB = 0xff;
		_delay_ms(1000);
    }
}


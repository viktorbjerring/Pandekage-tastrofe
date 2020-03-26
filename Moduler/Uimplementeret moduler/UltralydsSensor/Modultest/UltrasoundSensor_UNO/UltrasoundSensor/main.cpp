/*
 * UltrasoundSensor.cpp
 *
 * Created: 23/03/2020 21:34:26
 * Author : oskar
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdint.h>

#define F_CPU 16000000
#include <util/delay.h>

#include "Ultrasound.h"
#include "uart.h"


int main(void)
{
	char buffer[256];
	
	/* Enable interrupts */
	sei();
	
	DDRB |= (1<<5);
	bool isEmpty = false;
	
	InitUART(57600, 8, 'N');
	
    /* Replace with your application code */
    while (1) 
    {
		if (Ultrasound::GetDist() > 25 && !isEmpty)
		{
			isEmpty = true;
			PORTB |= (1<<5);
		}
		else if (Ultrasound::GetDist() < 10 && isEmpty)
		{
			isEmpty = false;
			PORTB &= ~(1<<5);
		}
		
		Ultrasound::StartReading();
		_delay_ms(100);
    }
}


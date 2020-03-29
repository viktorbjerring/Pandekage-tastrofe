/* 
* Ultrasound.cpp
*
* Created: 23/03/2020 21:35:48
* Author: oskar
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000
#include <util/delay.h>

#include "Ultrasound.h"
#include "uart.h"

int16_t dist_ = 0;
bool isEcho_ = false;
static bool readingReady_ = false;

ISR(INT0_vect)
{
	/* If currently timing PW */
	if (isEcho_)
	{
		/* Turn off timer0 */
		TCCR0B = 0;
		
		/* Calculate and save distance */
		uint8_t count_ = TCNT0;
		dist_ = static_cast<int16_t>(10*REGRESSION(TCNT0)); // Distance in cm
		
		readingReady_ = true;
		EIMSK &= ~(1<<INT0);
	}
	else
	{
		/* Reset and start timer0 */
		TCNT0 = 0;
		TCCR0B = 0b00000100;
		
		isEcho_ = true;
		/* Switch to falling edge */
		EICRA &= ~(1<<ISC00);
	}
}

int getBatterLevel()
{
	readingReady_ = false;
	isEcho_ = false;
	
	/* Set INT0 to trigger on rising edge */
	EIMSK |= (1<<INT0);
	EICRA |= ((1<<ISC01) | (1<<ISC00));
	
	PORTD |= (1<<3);
	_delay_us(10);
	PORTD &= ~(1<<3);
	
	_delay_ms(4);
	
	while(!readingReady_) {continue;}
	
	return dist_;
}

void Init()
{
	/* Setup trigger pin direction */
	DDRD |= (1<<3);
	DDRD |= (1<<4);
	
	/* Setup echo pin direction */
	DDRD &= ~(1<<2);
	
	/* Clear timer0 register A */
	TCCR0A = 0;
}


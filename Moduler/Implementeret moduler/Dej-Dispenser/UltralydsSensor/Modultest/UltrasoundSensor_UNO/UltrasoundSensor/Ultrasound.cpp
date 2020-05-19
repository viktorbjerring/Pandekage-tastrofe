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

uint16_t dist_ = 0;
bool isEcho_ = false;

ISR(INT0_vect)
{
	/* If currently timing PW */
	if (isEcho_)
	{
		/* Turn off timer2 */
		TCCR2B = 0;
		
		/* Calculate and save distance */
		dist_ = static_cast<uint16_t>(10*REGRESSION(TCNT2)); // Distance in cm
		
		EIMSK &= ~(1<<INT0);
	}
	else
	{
		/* Reset and start timer2 */
		TCNT2 = 0;
		TCCR2B = 0b00000110;
		
		isEcho_ = true;
		/* Switch to falling edge */
		EICRA &= ~(1<<ISC00);
	}
}

uint16_t getBatterLevel()
{
	isEcho_ = false;
	
	/* Set INT0 to trigger on rising edge */
	EIMSK |= (1<<INT0);
	EICRA |= ((1<<ISC01) | (1<<ISC00));
	
	PORTD |= (1<<3);
	_delay_us(10);
	PORTD &= ~(1<<3);
	
	_delay_ms(10);
	
	return dist_;
}

void Init()
{
	/* Setup trigger pin direction */
	DDRD |= (1<<3);
	
	/* Setup echo pin direction */
	DDRD &= ~(1<<2);
	
	/* Clear timer2 register A */
	TCCR2A = 0;
}


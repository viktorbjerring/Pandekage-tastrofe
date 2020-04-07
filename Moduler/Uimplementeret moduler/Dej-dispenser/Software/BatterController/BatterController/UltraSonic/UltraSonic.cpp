/*
 * UltraSonic.cpp
 *
 * Created: 06/04/2020 12.13.46
 *  Author: au615804
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>

#define F_CPU 16000000
#include <util/delay.h>

#include "UltraSonic.h"

uint16_t batterLevel_ = 0;
bool isEcho_ = false;

ISR(INT0_vect) {
	/* If currently timing PW */
	if (isEcho_)
	{
		/* Turn off timer2 */
		TCCR2B = 0;
		
		/* Calculate and save distance */
		batterLevel_ = static_cast<uint16_t>(10*REGRESSION(TCNT2)); // Distance in cm
		
		/* Turn off external interrupt */
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

void initUltrasonic() {
	DDRD |=  (1<<3); // Trigger pin
	DDRD &= ~(1<<2); // Echo pin
	TCCR0A = 0; // Clear timer0 register A
}

uint16_t getBatterLevel() {
	isEcho_ = false;
	
	/* Set INT0 to trigger on rising edge */
	EIMSK |= (1<<INT0);
	EICRA |= ((1<<ISC01) | (1<<ISC00));
	
	/* Pulse trigger pin */
	PORTD |= (1<<3);
	_delay_us(10);
	PORTD &= ~(1<<3);
	
	/* Wait for measurement */
	_delay_ms(10);
	
	return batterLevel_;
}
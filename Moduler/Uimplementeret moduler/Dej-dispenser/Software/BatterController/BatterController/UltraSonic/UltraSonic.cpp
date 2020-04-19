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

volatile uint16_t batterLevel_ = 0;
bool isEcho_ = false;

ISR(PCINT1_vect) {
	/* If currently timing PW */
	if (isEcho_)
	{
		/* Turn off timer2 */
		TCCR2B = 0;
		
		/* Calculate and save distance */
		batterLevel_ = static_cast<uint16_t>(10*REGRESSION(TCNT2)); // Distance in mm
		
		/* Turn off pin change 13 interrupt */
		PCMSK1 &= ~(1 << PCINT13);
	}
	else
	{
		/* Reset and start timer2 */
		TCNT2 = 0;
		TCCR2B = 0b00000110;
		
		isEcho_ = true;
	}
}

void initUltrasonic() {
	DDRD |=  (1<<PORTC1); // Trigger pin
	DDRD &= ~(1<<PINC2); // Echo pin
	TCCR2A = 0; // Clear timer2 register A
	PCICR |= (1 << PCIE1); // enable pin change interrupt 1
}

uint16_t readBatterLevel() {
	isEcho_ = false;
	
	/* Set PC1 to trigger on pin 13, PINC5 */
	PCMSK1 |= (1 << PCINT10);
	
	/* Pulse trigger pin */
	PORTD |= (1<<PORTD4);
	_delay_us(10);
	PORTD &= ~(1<<PORTD4);
	
	/* Wait for measurement */
	_delay_ms(10);
	
	return batterLevel_;
}
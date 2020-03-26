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

ISR(INT0_vect)
{
	Ultrasound::Interrupt();
}

float Ultrasound::dist_ = 0;
uint8_t Ultrasound::count_ = 0;
bool Ultrasound::isEcho_ = false;

/*
 * Create 10us pulse on trigger pin
**/
void Ultrasound::StartReading()
{
	PORTD |= (1<<6);
	_delay_us(10);
	PORTD &= ~(1<<6);
}

void Ultrasound::Init()
{
	/* Setup external interrupt */
	EICRA |= 0b11;
	EIMSK |= 0b1;
	
	/* Setup echo pin direction */
	DDRD |= (1<<6);
	
	/* Setup trigger pin direction */
	DDRD &= ~(1<<2);
	
	/* Clear timer0 register A */
	TCCR0A = 0;
}

void Ultrasound::Interrupt()
{
	/* If currently timing PW */
	if (isEcho_)
	{
		/* Turn off timer0 */
		TCCR0B = 0;
		
		/* Calculate and save distance */
		count_ = TCNT0;
		float dist = REGRESSION(TCNT0); // Distance in cm
		dist_ = dist;
		
		isEcho_ = false;
		/* Switch to rising edge */
		EICRA |= 0b1;
	}
	else
	{
		/* Reset and start timer0 */
		TCNT0 = 0;
		TCCR0B = 0b00000100;
		
		isEcho_ = true;;
		/* Switch to falling edge */
		EICRA &= ~0b1;
	}
}


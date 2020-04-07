/*
 * BatterDispenser.cpp
 *
 * Created: 06/04/2020 12.19.12
 *  Author: au615804
 */ 

#include "BatterDispenser.h"

#define cycleCount 5
#define extended 4000
#define retracted 2000
#define moveSpeed 200

volatile unsigned int cycle;
volatile unsigned int servoTop = extended;
volatile unsigned int servoBottom = retracted;

void initBatterDispenser() {
	cycle = 0;
	DDRB |= (1 << PORTB2) | (1 << PORTB3);
	// timer 1 fast PWM mode 50 Hz
	TCCR1A |= (1 << COM1B1) | (1 << WGM11); //B1 clear on compare match, top = ICR1
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); //Prescaler 8
	ICR1 = 39999; // freq 50 hz
	TIMSK1 |= (1 << TOIE1); // Enable overflow interrupt for timer 1
}


void addDough()
{
	servoTop = extended;
	_delay_ms(moveSpeed);
	servoBottom = retracted;
	_delay_ms(moveSpeed);
	servoBottom = extended;
	_delay_ms(moveSpeed);
	servoTop = retracted;
	return;
}

ISR(TIMER1_OVF_vect) {
	cycle++;
	if (cycle == cycleCount - 1) {
		OCR1B = servoBottom;
	} else if (cycle == cycleCount) {
		PORTB |= (1 << PORTB3);		
	} else if (cycle == cycleCount * 2 - 1) {
		OCR1B = servoTop;	
	} else if (cycle == cycleCount * 2) {
		cycle = 0;	
		PORTB &= ~(1 << PORTB3);
	}
}
/*
 * BatterDispenser.cpp
 *
 * Created: 06/04/2020 12.19.12
 *  Author: au615804
 */ 

#include "BatterDispenser.h"

static unsigned int cycle;

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
	return;
}

ISR(TIMER1_OVF_vect) {
	cycle++;
	if (cycle == 4) {
		OCR1B = 30000;
	} else if (cycle == 5) {
		PORTB |= (1 << PORTB3);		
	} else if (cycle == 9) {
		OCR1B = 10000;		
	} else if (cycle == 10) {
		cycle = 0;
		PORTB &= ~(1 << PORTB3);
	}
}
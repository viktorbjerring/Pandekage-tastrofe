/*
 * BatterCooler.cpp
 *
 * Created: 06/04/2020 12.22.06
 *  Author: au615804
 */ 

#include "BatterCooler.h"

volatile unsigned int temp = 0;
volatile bool isReady = false;

void beginCoolingRegulation()
{
	DDRB |= (1 << PORTB1);
	DDRC &= ~(1 << PINC3);
	// timer 1 fast PWM mode 50 Hz
	TCCR1A |= (1 << COM1A1) | (1 << WGM11); //A1 clear on compare match, top = ICR1
	TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); //Prescaler 8
	ICR1 = 39999; // freq 50 hz
	OCR1A = 20000;
	// timer 0 CTC mode, 100 hz
	TCNT0 = 0;
	TCCR0A |= (1 << WGM01); // Set CTC mode
	TCCR0B |= (1 << CS00) | (1 << CS02); // Prescaler 1024
	OCR0A = 67; // Compare register
	// ADC init
	ADCSRA |= (1 << ADPS1); // Set ADC prescaler to 4
	ADMUX |= (1 << ADLAR) | (1 << MUX0) | (1 << MUX1); // Left shift ADC reg, ADC port 3
	ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADIE); // ADC enable, autotrigger and interrupt enable
	ADCSRB |= (1 << ADTS1) | (1 << ADTS0); // set autotrigger to timer0 compare A
}

unsigned int returnTemp() {
	while (!isReady) {
		_delay_ms(10);
	}
	isReady = false;
	return temp;
}

ISR(ADC_vect) {
	TIFR0 |= (1 << OCF0A);
	temp = ADC >> 6; // Read ADC
	isReady = true;
	// Set OCR1A for PWM.
	//ADCSRA |= (1 << ADIF); //Clear interrupt
}

/*
 * PanController.c
 *
 * Created: 19/04/2020 15.19.19
 * Author : Mikkel
 */ 

#include <avr/io.h>
#include "board.h"
#include "Digital_protocol.h"
#include "heat_reg.h"
#include "I2C_SLAVE.h"
#include "Motor.h"

void init_1Hz_timer();

int main(void)
{
	//Enable interrupts
	sei();
	
	init_1Hz_timer();
	init_digital_comm();		//Initialize the digital communication
	
	DDRB |= (1 << 5);

    while (1) 
    {
		if (heat_on_ctrl) {
			PORTB |= (1 << PORTB5);
		} else {
			PORTB &= ~(1 << PORTB5);
		}
		
		
    }
	
}

void init_1Hz_timer(){
	//Setup timer
	PRR &= ~(1 << PRTIM1);	//Enable timer 1
	
	 
	//Timer is in fast PWM mode, TOP is ICR1 and prescaler is 256
	TCCR1A = (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12);
	
	//Set TOP value
	ICR1 = TIMER_1HZ_TOP;
	
	//Overflow interrupt enabled
	TIMSK1 = (1 << TOIE1);
}

ISR(TIMER1_OVF_vect){
	
	//Control cooking time
	pan1_cooking_time += 1;
	
	if (pan1_cooking_time == 4){
		pancake_done = true;
		pan1_cooking_time = 0;
	}
	
	//Pancake Done
	pancakeDone();
	
}

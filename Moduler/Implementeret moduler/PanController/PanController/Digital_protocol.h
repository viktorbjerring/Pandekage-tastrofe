/*
 * Digital_protocol.h
 *
 * Created: 19/04/2020 15.55.51
 *  Author: Mikkel
 */ 


#ifndef DIGITAL_PROTOCOL_H_
#define DIGITAL_PROTOCOL_H_

#include "board.h"

extern volatile bool heat_on_ctrl;

//PAN_ON_OFF interrupt
// pin for heat on/off is B0
#define HEAT_ON_OFF_PIN		(1 << PINB0)		//PB0
#define PANCAKE_DONE_PIN	(1 << PORTB4)		//PB4

#define DIGITAL_PROTOCOL_vect	PCINT0_vect

void init_digital_comm() {
	//Pin setup
	//Set Heat on/off as input and PANCAKE_DONE as output
	DDRB &= ~(HEAT_ON_OFF_PIN);
	DDRB |= PANCAKE_DONE_PIN;	
	
	PORTB &= ~(PANCAKE_DONE_PIN | HEAT_ON_OFF_PIN);		//Set PANCAKE_DONE line to low and no pullup for heat on/off
	
	//Interrupt setup
	PCICR |= (1<<PCIE0);						//Enable pin change interrupt 0
	PCMSK0 |=  (1 << PCINT0);			//Set pin B0 as an interrupt pin
	
}

void pancakeDone(){
	
	if (pancake_done) {
		PORTB |= PANCAKE_DONE_PIN;
		pancake_done = false;
	}
	else {
		PORTB &= ~(PANCAKE_DONE_PIN);
	}

}

static void turnOnPans() {
	if(PINB & HEAT_ON_OFF_PIN){
		heat_on_ctrl = !heat_on_ctrl;		//Toggle the heat control variable
	}
}

//PAN_ON/OFF interrupt
ISR(DIGITAL_PROTOCOL_vect){
	turnOnPans();
}



#endif /* DIGITAL_PROTOCOL_H_ */
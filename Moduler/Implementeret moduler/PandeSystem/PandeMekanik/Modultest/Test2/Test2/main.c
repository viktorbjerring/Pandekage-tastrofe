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
	//init_1Hz_timer();
	init_motors();				//Initialize the motor pins and PWMs
	pan1_cooking_time = 0;
    while (1) 
    {	
		_delay_ms(1000);
		pan1_cooking_time++;	
	
		if (pan1_cooking_time >= 10) {
			pan1_cooking_time = 0;
		}
		
		if (pan1_cooking_time == 0){
			flipPan1();
		}
		
		if (pan1_cooking_time == 3){
			flipPan2();
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
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

//Set main clock to 8 MHz
FUSES = {
	.low = 0x12,			//Internal occilator at 8 MHz and startup time at fastest possible
	.high = HFUSE_DEFAULT,
	.extended = EFUSE_DEFAULT,
};

void init_1Hz_timer();

int main(void)
{
	//Enable interrupts
	sei();
	init_1Hz_timer();
	
	init_digital_comm();		//Initialize the digital communication
	I2C_SLAVE_init();			//Initialize the I2C communication
	init_regulation();			//Initialize the regulation
	init_motors();				//Initialize the motor pins and PWMs
	
    /* Replace with your application code */
    while (1) 
    {
		if (pan1_cooking_time == PANCAKE_COOKING_TIME1_S){
			flipPan1();
		}
		
		if (pan2_cooking_time == PANCAKE_COOKING_TIME2_S){
			flipPan2();
		}
		
		if (I2C_SLAVE_checkData()){
			I2C_commands_t temp = I2C_SLAVE_getData();
			switch (temp) {
				
				case PING:
					I2C_SLAVE_sendData(temp);
					break;
				case GET_FIRST_PAN_STATUS:
					I2C_SLAVE_sendData(pan1Free & heat_ok);
					break;
				case BEGIN_COOCKING:
					startTimePan1();
					break;
				default:
					break;
			}	
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
	pan1_cooking_time += (pan1Free ? 0 : 1);
	pan2_cooking_time += (pan2Free ? 0 : 1);
	
	//Pancake Done
	pancakeDone();
	
}

/*
 * heat_reg.h
 *
 * Created: 19/04/2020 16.42.14
 *  Author: Mikkel
 */ 


#ifndef HEAT_REG_H_
#define HEAT_REG_H_

#include "board.h"

extern volatile bool heat_on_ctrl;
extern volatile bool heat_ok;

/*
	Probename is:
	PT1000 temperature probe

	With TCR-3850ppm/K
	And R0 = 1000 Ohms
	
	Link to temperature probe table:
	https://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=20&ved=2ahUKEwjmpeOb5PToAhWRp4sKHZYyBBcQFjATegQIBhAB&url=https%3A%2F%2Fwww.vdhproducts.com%2Fwp-content%2Fuploads%2F2015%2F01%2Fsm8000en.pdf&usg=AOvVaw3Epf1Ew6G6GoOt1zOZ35AZ

	Se temp beregning under HW for specificering
	
*/

#define CONVERT_TO_STEP(x)	((uint16_t) (18152*((double)x)-4650000)/(3.9*((double)x)+7200))
#define CONVERT_TO_TEMP(x)	((double) -((1.71*((double)x))/(0.000107*((double)x)-5))-256)
#define CONVERT_TO_DUTY(x)	((uint8_t) (((double)x)-96.29)/(1.432))


#define TRIGGER_LOW			CONVERT_TO_STEP(182)		
#define TRIGGER_HIGH		CONVERT_TO_STEP(188)		
#define OPTIMUM_TEMP		CONVERT_TO_DUTY(CONVERT_TO_TEMP(TRIGGER_LOW + (TRIGGER_HIGH - TRIGGER_LOW)/2))


void init_regulation(){
	
	//Setup ADC
	PRR &= ~(1 << PRADC);	//Power management enabling of adc
	DDRC &= 0xFC;			//Port C 0 and 1 is input
	ADMUX =	(1 << REFS0);	//REF = VCC, ADC0 selected
	
	ADCSRA |= (1 << ADEN) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);	//Enable ADC, Enable ADC convertion interrupt , prescaler = 128
	
	//Setup for power PWM using OCR1B and OCR1A
	DDRB |= (1 << PORTB2) | (1 << PORTB1);	//Output for power PWM at PB2 and PB1
	
	OCR1A = TIMER_1HZ_TOP;			//Set PWM level to 0 for pan1
	OCR1B = TIMER_1HZ_TOP;			//Set PWM level to 0 for pan2
	
	//OCR1A and B is set on compare match and cleared at bottom (0)
	TCCR1A |= (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0);;;;;;
	
	//Start the regulation loop
	ADCSRA |= (1 << ADSC);
	
}


#define PAN1	false
#define PAN2	true


static volatile bool curr_pan = PAN1;

static void setPWMLevel(uint16_t PWM_level){	//Sets the PWM level for the heating
	
	if (curr_pan)
		OCR1B = ceil(TIMER_1HZ_TOP - ((TIMER_1HZ_TOP/100) * (PWM_level > 100 ? 100 : PWM_level)));
	else
		OCR1A = ceil(TIMER_1HZ_TOP - ((TIMER_1HZ_TOP/100) * (PWM_level > 100 ? 100 : PWM_level)));
}

static uint16_t readHeatLevel(){
	
	//Read ADCL first - it is important
	uint8_t temp = ADCL;
	return ((ADCH & 0x03) << 8) + temp;
}

static volatile double integral = 0;

#define KP		((double) 3)
#define KI		((double) 75)
#define DT		((double)((128*13)/F_CPU)) //Time for single convertion ~ 1/16000000/(128*13)	((one convertion = 13 clock cyckles) * prescaler)/F_CPU)
#define HYSTERESIS_HIGH		CONVERT_TO_STEP(170)
#define HYSTERESIS_LOW		CONVERT_TO_STEP(130)

static bool has_been_on = false;

//Regulation loop - not time critical
ISR(ADC_vect)
{
	
	uint16_t temp = readHeatLevel();
	
	
	//Check heat level
	if (curr_pan == PAN1) {
		if (temp >= TRIGGER_LOW && temp <= TRIGGER_HIGH){
			heat_ok_pan1 = true;
		} else {
			heat_ok_pan1 = false;
		}
	} else {
		if (temp >= TRIGGER_LOW && temp <= TRIGGER_HIGH){
			heat_ok_pan2 = true;
		} else {
			heat_ok_pan2 = false;
		}
	}
	heat_ok = heat_ok_pan1 && heat_ok_pan2;
	
	//If no heating control - turn off the pans
	if (!heat_on_ctrl)		
	{
		integral = 0;
		setPWMLevel(0);
	} else {
		
		if (temp > HYSTERESIS_HIGH){
			has_been_on = true;
		}
		else if (temp < HYSTERESIS_LOW){
			integral = 0;
			has_been_on = false;
		}
		
		if (has_been_on) {
			
			uint8_t duty = CONVERT_TO_DUTY(CONVERT_TO_TEMP(temp));
			
			
			//Calculate the values for the PI controller
			int16_t error = OPTIMUM_TEMP - duty;
			integral += error*DT;
					
			//Calculate the output
			int16_t output = error*KP + integral*KI;
					
			//Set PWM
			setPWMLevel((output < 0? 0 : output));
		}
		else {
			setPWMLevel(100);
		}
	

	}
	//Change pan
	curr_pan = !curr_pan;
		
	ADMUX ^= 0x01;		//Change between ADC0 and ADC1
	
	//Restart conversion
	ADCSRA |= (1 << ADSC);
}

#endif /* HEAT_REG_H_ */
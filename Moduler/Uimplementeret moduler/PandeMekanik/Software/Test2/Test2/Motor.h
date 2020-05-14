/*
 * Motor.h
 *
 * Created: 19/04/2020 15.37.55
 *  Author: Mikkel
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#include "board.h"
#include "heat_reg.h"
#include "Digital_protocol.h"

extern volatile bool heat_ok;
extern volatile bool pan1Free;
extern volatile bool pan2Free;

typedef enum {
	//Motor states
	MOTOR_BREAK = 0x00,		//LL
	MOTOR_FORWARD = 0x01,	//LH
	MOTOR_BACKWARD = 0x02,	//HL
	MOTOR_OFF = 0x03,		//HH
} motor_cmd_t;

#define MOTOR1_DIR_PIN_LOCATION1 PORTD3
#define MOTOR1_DIR_PIN_LOCATION2 PORTD7
#define MOTOR2_DIR_PIN_LOCATION1 PORTC4
#define MOTOR2_DIR_PIN_LOCATION2 PORTC5
#define MOTOR1_EN_PIN_LOCATION   PORTD5
#define MOTOR2_EN_PIN_LOCATION   PORTD6

#define MOTOR1		false
#define	MOTOR2		true


static void setMotorPWM(uint8_t PWM, bool motor);


//Direction ports
#define MOTOR1_DIR_PORT_DDR		DDRD
#define MOTOR2_DIR_PORT_DDR		DDRC
#define MOTOR1_DIR_PORT			PORTD
#define MOTOR2_DIR_PORT			PORTC

//Enable ports
#define MOTOR1_ENABLE_PORT_DDR	DDRD
#define MOTOR1_ENABLE_PORT		PORTD
#define MOTOR2_ENABLE_PORT_DDR	DDRD
#define MOTOR2_ENABLE_PORT		PORTD

//Macros
#define MOTOR1_SETING1(x)	MOTOR1_DIR_PORT = ((MOTOR1_DIR_PORT & ~(1 << MOTOR1_DIR_PIN_LOCATION1)) | (x << MOTOR1_DIR_PIN_LOCATION1))
#define MOTOR1_SETING2(x)	MOTOR1_DIR_PORT = ((MOTOR1_DIR_PORT & ~(1 << MOTOR1_DIR_PIN_LOCATION2)) | (x << MOTOR1_DIR_PIN_LOCATION2))
#define MOTOR1_SETING(x)	{ MOTOR1_SETING1((x & 0b01)>>0); MOTOR1_SETING2((x & 0b10)>>1);}
#define MOTOR2_SETING1(x)	MOTOR2_DIR_PORT = ((MOTOR2_DIR_PORT & ~(1 << MOTOR2_DIR_PIN_LOCATION1)) | (x << MOTOR2_DIR_PIN_LOCATION1))
#define MOTOR2_SETING2(x)	MOTOR2_DIR_PORT = ((MOTOR2_DIR_PORT & ~(1 << MOTOR2_DIR_PIN_LOCATION2)) | (x << MOTOR2_DIR_PIN_LOCATION2))
#define MOTOR2_SETING(x)	{ MOTOR2_SETING1((x & 0b01)>>0); MOTOR2_SETING2((x & 0b10)>>1);}

//OCR0A/B set when counting up, duty depends on OCR0A/B,
#define MOTOR1_DISABLE()	(TCCR0A &= ~((1 << COM0B1) | (1 << COM0B0)))
#define MOTOR2_DISABLE()	(TCCR0A &= ~((1 << COM0A1) | (1 << COM0A0)))
#define MOTOR1_ENABLE()		(TCCR0A |= ((1 << COM0B1) | (1 << COM0B0)))
#define MOTOR2_ENABLE()		(TCCR0A |= ((1 << COM0A1) | (1 << COM0A0)))

//Timer defs
#define TIMER0_TOP		0xFF
#define TIMER2_TOP		250-1

static void startTimer2();
static void stopTimer2();
static void startTimePan2();

void init_motors(){
	
	//Set pins as output
	MOTOR1_DIR_PORT_DDR |= (1 << MOTOR1_DIR_PIN_LOCATION1) | (1 << MOTOR1_DIR_PIN_LOCATION2);
	MOTOR2_DIR_PORT_DDR |= (1 << MOTOR2_DIR_PIN_LOCATION1) | (1 << MOTOR2_DIR_PIN_LOCATION2);
	
	//Set enable pins to output low as default
	MOTOR1_ENABLE_PORT_DDR |= (1 << MOTOR1_EN_PIN_LOCATION);
	MOTOR2_ENABLE_PORT_DDR |= (1 << MOTOR2_EN_PIN_LOCATION);
	MOTOR1_ENABLE_PORT &= ~(1 << MOTOR1_EN_PIN_LOCATION);
	MOTOR2_ENABLE_PORT &= ~(1 << MOTOR2_EN_PIN_LOCATION);
	
	//Set motor to off
	MOTOR1_SETING(MOTOR_OFF);
	MOTOR2_SETING(MOTOR_OFF);
	
	//Disable motors
	MOTOR1_DISABLE();
	MOTOR2_DISABLE();
	
		
	//Setup timer 0 PWMs for the motors
	PRR &= ~(1 << PRTIM0);
	
	// phase correct PWM mode, prescaler = 8, freq ~ 3921,5686 Hz (16000000/(8*510)
	TCCR0A = (1 << WGM00);
	TCCR0B |= (1 << CS02);
	
	//Set PWM for the motors
	setMotorPWM(0, MOTOR1);
	setMotorPWM(0, MOTOR2);
	
	//Setup timer 2 to 125 Hz and TOF interrupt, prescaler = 256, OCRA = 249 (16000000 Hz/(256*250) = 250 Hz (error = 125 +- 1Hz)
	PRR &= ~(1 << PRTIM2);
	TCCR2A = (1 << WGM21) | (1 << WGM20);
	TCCR2B = (1 << WGM22);
	OCR2A = TIMER2_TOP;
	TIMSK2 = (1 << TOIE2);
	
}

void startTimePan1(){
	
	//Set internal flag
	pan1_cooking_time = 0;
	pan1Free = false;
	
	
	
}

static void startTimePan2(){
	
	//Set internal flag
	pan2_cooking_time = 0;
	pan2Free = false;
	
}

static volatile uint32_t curr_time = 0;	
static volatile bool motor_flipping = MOTOR1;

static void setMotorPWM(uint8_t PWM, bool motor) {
	if (motor == MOTOR1) {
		OCR0A = (TIMER0_TOP - ceil(((TIMER0_TOP/100) * (PWM))));
	}
	else {
		OCR0B = (TIMER0_TOP - ceil(((TIMER0_TOP/100) * (PWM))));
	}
}

void flipPan1(){
	
	motor_flipping = MOTOR1;
	setMotorPWM(MOTOR_OPTIMUM_PWM_FORWARD, motor_flipping);
	
	//Flip forward
	MOTOR1_SETING(MOTOR_FORWARD);
	MOTOR1_ENABLE();
	
	startTimer2();
}

void flipPan2(){
	
	motor_flipping = MOTOR2;
	setMotorPWM(MOTOR_OPTIMUM_PWM_FORWARD, motor_flipping);
	
	//Flip forward
	MOTOR2_SETING(MOTOR_FORWARD);
	MOTOR2_ENABLE();
	
	startTimer2();
	
}

static void startTimer2(){
	curr_time = 0;
	TCCR2B |= (1 << CS22) | (1 << CS20);
}

static void stopTimer2(){
	//Stop clock
	TCCR2B &= ~((1 << CS22) | (1 << CS20));
	
	//Reset count
	TCNT2 = 0x00;
}

ISR(TIMER2_OVF_vect) {
	
	switch (curr_time++) {
		
		//Mid way time to break
		case MOTOR_FORWARD_TIME_S:
		case MOTOR_BACKWARD_TIME_S:
			if (motor_flipping == MOTOR1) {
				MOTOR1_DISABLE();
				OCR0A = 125;
				//setMotorPWM(MOTOR_OPTIMUM_PWM_BREAK, motor_flipping);
				MOTOR1_SETING(MOTOR_BREAK);
				MOTOR1_ENABLE();
			}
			else {
				MOTOR2_DISABLE();
				setMotorPWM(MOTOR_OPTIMUM_PWM_BREAK, motor_flipping);
				MOTOR2_SETING(MOTOR_BREAK);
				MOTOR2_ENABLE();
			}
			break;
		
		//We have stopped the pan at pancake destination time to go back
		case MOTOR_BREAK_FORWARD_TIME_S:
			if (motor_flipping == MOTOR1) {
				MOTOR1_DISABLE();
				OCR0A = 0;
				//setMotorPWM(MOTOR_OPTIMUM_PWM_BACKWARD, motor_flipping);
				MOTOR1_SETING(MOTOR_BACKWARD);
				MOTOR1_ENABLE();
				startTimePan2();
			}
			else {
				MOTOR2_DISABLE();
				setMotorPWM(MOTOR_OPTIMUM_PWM_BACKWARD, motor_flipping);
				MOTOR2_SETING(MOTOR_BACKWARD);
				MOTOR2_ENABLE();
			}
			break;
		
		//We have reached the starting position set flags
		case MOTOR_BREAK_BACKWARD_TIME_S:
			if (motor_flipping == MOTOR1) {
				MOTOR1_DISABLE();
				MOTOR1_SETING(MOTOR_OFF);
				pan1Free = true;
			}
			else {
				MOTOR2_DISABLE();
				MOTOR2_SETING(MOTOR_OFF);
				pancake_done = true;
				pan2Free = true;
			}
			//Stop the timer
			stopTimer2();
			break;
	}
}


#endif /* MOTOR_H_ */
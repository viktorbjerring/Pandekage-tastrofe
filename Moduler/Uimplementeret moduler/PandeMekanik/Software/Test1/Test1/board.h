/*
 * board.h
 *
 * Created: 19/04/2020 15.19.34
 *  Author: Mikkel
 */ 

#include <stdbool.h>
#include <avr/interrupt.h>
#include <math.h>

#ifndef BOARD_H_
#define BOARD_H_

extern volatile bool heat_on_ctrl;
extern volatile bool heat_ok_pan1;
extern volatile bool heat_ok_pan2;
extern volatile bool heat_ok;
extern volatile bool pancake_done;
extern volatile uint16_t pan1_cooking_time;
extern volatile uint16_t pan2_cooking_time;
extern volatile bool pan1Free;
extern volatile bool pan2Free;

#define F_CPU				16000000		//16 MHZ


#define TIMER_1HZ_TOP					62499	//TOP = (1600000/256) - 1 = 62.499
#define TIMER2_COUNTS_PR_SECOND			250		//Timer 2 has a freq of 250 Hz

//Baking time

#define PANCAKE_COOKING_TIME1_S			25		//Cooking time for first side of pancake accurate to +- 1 second
#define PANCAKE_COOKING_TIME2_S			10		//Cooking time for second side of pancake accurate to +- 1 second

//Motor timing

#define MOTOR_OPTIMUM_PWM_FORWARD		33		//33% duty cycle
#define MOTOR_OPTIMUM_PWM_BACKWARD		66		//66% duty cycle
#define MOTOR_OPTIMUM_PWM_BREAK			100		//100% duty cycle

//NOTE max 166 seconds total
#define MOTOR_FORWARD_TIME_S			TIMER2_COUNTS_PR_SECOND * 0x04								//4 seconds
#define MOTOR_BREAK_FORWARD_TIME_S		MOTOR_FORWARD_TIME_S + TIMER2_COUNTS_PR_SECOND * 0x03		//3 seconds
#define MOTOR_BACKWARD_TIME_S			MOTOR_BREAK_FORWARD_TIME_S + TIMER2_COUNTS_PR_SECOND * 0x04		//4 seconds
#define MOTOR_BREAK_BACKWARD_TIME_S		MOTOR_BACKWARD_TIME_S + TIMER2_COUNTS_PR_SECOND * 0x03		//3 seconds

//I2C
#define I2C_SLAVE_ADDR				0x70
#define I2C_SLAVE_BUFFER_LENGTH		10

#endif /* BOARD_H_ */
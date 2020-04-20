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

extern bool heat_on_ctrl;
extern bool heat_ok;
extern bool pancake_done;
extern uint16_t pan1_cooking_time;
extern uint16_t pan2_cooking_time;
extern bool pan1Free;
extern bool pan2Free;

#define F_CPU				8000000		//8 MHZ


#define TIMER_1HZ_TOP					0x7A11	//TOP = (800000/256) - 1 = 31249
#define TIMER2_COUNTS_PR_SECOND			125		//Timer 2 has a freq of 125 Hz

//Baking time

#define PANCAKE_COOKING_TIME1_S			180		//Cooking time for first side of pancake accurate to +- 1 second
#define PANCAKE_COOKING_TIME2_S			120		//Cooking time for second side of pancake accurate to +- 1 second

//Motor timing

#define MOTOR_OPTIMUM_PWM_FORWARD		100		//100% duty cycle
#define MOTOR_OPTIMUM_PWM_BACKWARD		100		//100% duty cycle
#define MOTOR_OPTIMUM_PWM_BREAK			100		//100% duty cycle

//NOTE max 332 seconds total
#define MOTOR_FORWARD_TIME_S			TIMER2_COUNTS_PR_SECOND * 0x04								//4 seconds
#define MOTOR_BREAK_FORWARD_TIME_S		MOTOR_FORWARD_TIME_S + TIMER2_COUNTS_PR_SECOND * 0x03		//3 seconds
#define MOTOR_BACKWARD_TIME_S			MOTOR_BREAK_FORWARD_TIME_S + TIMER2_COUNTS_PR_SECOND * 0x04		//4 seconds
#define MOTOR_BREAK_BACKWARD_TIME_S		MOTOR_BACKWARD_TIME_S + TIMER2_COUNTS_PR_SECOND * 0x03		//3 seconds

//I2C
#define I2C_SLAVE_ADDR				0x70
#define I2C_SLAVE_BUFFER_LENGTH		10

#endif /* BOARD_H_ */
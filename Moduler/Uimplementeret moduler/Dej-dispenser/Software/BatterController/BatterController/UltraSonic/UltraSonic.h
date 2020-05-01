/*
 * UltraSonic.h
 *
 * Created: 06/04/2020 12.16.50
 *  Author: au615804
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_


/*------- Includes -------*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#define F_CPU 16000000
#include <util/delay.h>
/*------------------------*/


/*------- Macros -------*/
#define REGRESSION(x) (0.2779 * x - 0.2383)
/*----------------------*/


/*------- Register definitions -------*/
#define ULTRSNC_TRGGR_DDR		DDRC
#define ULTRSNC_TRGGR_PORT		PORTC
#define ULTRSNC_TRGGR_PORT_NUM	PORTC1

#define ULTRSNC_ECHO_DDR		DDRC
#define ULTRSNC_ECHO_PORT		PORTC
#define ULTRSNC_ECHO_PIN_NUM	PINC2

#define ULTRSNC_TCCRA	TCCR2A
#define ULTRSNC_TCCRB	TCCR2B
#define ULTRSNC_TCNT	TCNT2

#define ULTRSNC_PCIE	PCIE1
#define ULTRSNC_PCMSK	PCMSK1
#define ULTRSNC_PCINT	PCINT13
/*------------------------------------*/


/*------- Batter-Container definitions -------*/
#define CNTNR_W 100		// Container width in mm
#define CNTNR_L 100		// Container length in mm
#define CNTNR_H 100		// Container heigth in mm
#define DSG_VOL 100		// Dosage volume in ml
/*--------------------------------------------*/


/*------- Prototypes -------*/

/* Initializes Ultrasonic sensor */
void initUltrasonic();

/* Get amount of pancakes that can be made with current amount of batter */
int readBatterAmount();

/*--------------------------*/


#endif /* ULTRASONIC_H_ */
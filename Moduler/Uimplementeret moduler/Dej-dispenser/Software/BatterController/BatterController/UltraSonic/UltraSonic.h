/*
 * UltraSonic.h
 *
 * Created: 06/04/2020 12.16.50
 *  Author: au615804
 */ 


#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

#include "UltraSonic.h"

#include <stdint.h>

#define REGRESSION(x) (0.2779 * x - 0.2383)

/* Initializes Ultrasonic sensor */
void initUltrasonic();

/* Pulses trigger pin and returns measured distance in mm */
uint16_t readBatterLevel();


#endif /* ULTRASONIC_H_ */
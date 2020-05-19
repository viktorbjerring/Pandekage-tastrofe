/*
 * BatterCooler.h
 *
 * Created: 06/04/2020 12.22.43
 *  Author: au615804
 */ 


#ifndef BATTERCOOLER_H_
#define BATTERCOOLER_H_

#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void beginCoolingRegulation();
void endCoolingRegulation();
unsigned int returnTemp();
#endif /* BATTERCOOLER_H_ */
/*
 * BatterDispenser.h
 *
 * Created: 06/04/2020 12.19.33
 *  Author: au615804
 */ 


#ifndef BATTERDISPENSER_H_
#define BATTERDISPENSER_H_

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define Dispense_time 10

void initBatterDispenser();

void addDough();

#endif /* BATTERDISPENSER_H_ */
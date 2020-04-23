/*
 * BatterController.cpp
 *
 * Created: 06/04/2020 12.06.58
 * Author : au615804
 */ 

#include <avr/io.h>
#include <stdio.h>

#define F_CPU 16000000
#include <util/delay.h>

#include "main.h"
#include "uart.h"
#include "BatterCooler/BatterCooler.h"
#include "BatterDispenser/BatterDispenser.h"
#include "ControlUnit/ControlUnit.h"
#include "UltraSonic/UltraSonic.h"
#include "PanController/PanController.h"


int main()
{
	sei();
	
	initControlUnit();
	initPanController();
	initUltrasonic();
	initBatterDispenser();
	
	/* Initialize indicator */
	INDCTR_DDR |= (1<<INDCTR_PORT_NUM);
	
    while (1) 
    {
		slavePoll();
		_delay_ms(500);
    }
}

void pancakeBegin() {
	waitForFreePan();
	addDough();
	cookingBegin();
}

int getBatterAmount() {
	int level = readBatterAmount();
	
	if (level > MIN_BATTER_LEVEL)
		INDCTR_PORT |=  (1<<INDCTR_PORT_NUM);
	else
		INDCTR_PORT &= !(1<<INDCTR_PORT_NUM);
	
	return level;
}

void turnOnCooling() {
	beginCoolingRegulation();
}

void turnOffCooling() {
	
}

void turnOffAlarm() {
	
}


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
    }
}

void pancakeBegin() {
	waitForFreePan();
	//addDough();
	cookingBegin();
}

int getBatterAmount() {
	int level = readBatterAmount();
	
	if (level > MIN_BATTER_LEVEL)
		INDCTR_PORT |=  (1<<INDCTR_PORT_NUM); // Turn on indicator LED
	
	return level;
}

void turnOnCooling() {
	beginCoolingRegulation();
}

void turnOffCooling() {
	endCoolingRegulation();
}

void turnOffAlarm() {
	INDCTR_PORT &= ~(1<<INDCTR_PORT_NUM); // Turn off indicator LED
}


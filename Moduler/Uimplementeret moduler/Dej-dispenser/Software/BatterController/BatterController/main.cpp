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
	
    /* Replace with your application code */
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

int getBatterLevel() {
	return readBatterLevel();
}

void turnOnCooling() {
	beginCoolingRegulation();
}


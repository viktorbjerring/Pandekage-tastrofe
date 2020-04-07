/*
 * BatterController.cpp
 *
 * Created: 06/04/2020 12.06.58
 * Author : au615804
 */ 

#include <avr/io.h>
#include <stdio.h>

#include "uart.h"
#include "BatterCooler/BatterCooler.h"
#include "BatterDispenser/BatterDispenser.h"
#include "ControlUnit/ControlUnit.h"
#include "UltraSonic/UltraSonic.h"
#include "PanController/PanController.h"


int main(void)
{
	beginCoolingRegulation();
	//initBatterDispenser();
	InitUART(57600, 8, 'n');
	sei();
	char buf[32];
	unsigned int val = 1000;
    /* Replace with your application code */
    while (1) 
    {
		val = returnTemp();
		sprintf(buf, "%u\r\n", val);
		SendString(buf);
		_delay_ms(1000);
    }
}

/*
// UC2
void pancakeBegin();

// UC3
int getBatterLevel();

// UC4
void turnOnCooling();
*/
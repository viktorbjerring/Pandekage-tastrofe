/*
 * BatterController.cpp
 *
 * Created: 06/04/2020 12.06.58
 * Author : au615804
 */ 

#include <avr/io.h>

#include "BatterCooler/BatterCooler.h"
#include "BatterDispenser/BatterDispenser.h"
#include "ControlUnit/ControlUnit.h"
#include "UltraSonic/UltraSonic.h"
#include "PanController/PanController.h"


int main(void)
{
	beginCoolingRegulation();
	initBatterDispenser();
	sei();
    /* Replace with your application code */
    while (1) 
    {
		_delay_ms(10);
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
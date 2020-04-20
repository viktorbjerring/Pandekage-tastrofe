/*
 * ControlUnit.cpp
 *
 * Created: 06/04/2020 12.18.01
 *  Author: au615804
 */ 

#include "ControlUnit.h"
#include "../main.h"
#include "../I2C/I2C_SLAVE.h"

void initControlUnit()
{
	I2C_SLAVE_init();
}

void slavePoll() {
	if (I2C_SLAVE_checkData()) {
		I2C_commands_t cmd = I2C_SLAVE_getData();
		
		switch (cmd) {
			case MAKE_PANCAKE:
				pancakeBegin();
				break;
				
			case TURN_ON_COOLING:
				turnOnCooling();
				break;
			
			case GET_BATTER_LEVEL:
				int level = getBatterLevel();
				I2C_SLAVE_sendData(static_cast<char>(level));
				break;
		}
	}
}
/*
 * PanController.cpp
 *
 * Created: 06/04/2020 12.11.53
 *  Author: au615804
 */ 

#include "PanController.h"
#include "../I2C/I2C_MASTER.h"

void initPanController() {
	I2C_MASTER_init(I2C_BIT_RATE, I2C_MASTER_PRESCALE_64);
}

void waitForFreePan() {
	I2C_MASTER_sendData(I2C_PAN_ADDR, GET_FIRST_PAN_STATUS);
}

void cookingBegin() {
	I2C_MASTER_sendData(I2C_PAN_ADDR, BEGIN_COOCKING);
}
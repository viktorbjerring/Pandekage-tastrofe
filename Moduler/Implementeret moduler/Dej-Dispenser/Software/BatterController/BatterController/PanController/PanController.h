/*
 * PanController.h
 *
 * Created: 06/04/2020 12.12.21
 *  Author: au615804
 */ 


#ifndef PANCONTROLLER_H_
#define PANCONTROLLER_H_

#include "../I2C/I2C_common.h"

#define I2C_BIT_RATE 50
#define I2C_PAN_ADDR I2C_SLAVE_ADDR

/* Initializes I2C connection */
void initPanController();

/* Blocks until Pan-Controller returns ACK; this means that the first pan is available */
void waitForFreePan();

/* Signals Pan-Controller that a pancake has been dispensed */
void cookingBegin();

#endif /* PANCONTROLLER_H_ */
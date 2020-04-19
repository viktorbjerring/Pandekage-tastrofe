/*
 * PanController.h
 *
 * Created: 06/04/2020 12.12.21
 *  Author: au615804
 */ 


#ifndef PANCONTROLLER_H_
#define PANCONTROLLER_H_

#define I2C_BIT_RATE 57600
#define I2C_PAN_ADDR 0x22

/* Initializes I2C connection */
void initPanController();

/* Blocks until Pan-Controller returns ACK; this means that the first pan is available */
void waitForFreePan();

/* Signals Pan-Controller that a pancake has been dispensed */
void cookingBegin();

#endif /* PANCONTROLLER_H_ */
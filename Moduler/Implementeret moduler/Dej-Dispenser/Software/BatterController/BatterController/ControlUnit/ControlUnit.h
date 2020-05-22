/*
 * ControlUnit.h
 *
 * Created: 06/04/2020 12.18.20
 *  Author: au615804
 */ 


#ifndef CONTROLUNIT_H_
#define CONTROLUNIT_H_

#define I2C_MASTER_ADDR 0x45

/* Initialize Control Unit */
void initControlUnit();

/* Poll for slave command */
void slavePoll();

#endif /* CONTROLUNIT_H_ */
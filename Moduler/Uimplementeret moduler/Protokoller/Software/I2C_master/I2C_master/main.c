/*
 * I2C_master.c
 *
 * Created: 31/03/2020 17.24.16
 * Author : Mikkel
 */ 

#include <avr/io.h>
#include <avr/delay.h>
#include <stdio.h>

#include "I2C_MASTER.h"


int main(void)
{
	//Init master clock to 250 kHz
	I2C_MASTER_init(4U, I2C_MASTER_PRESCALE_1);
	
    /* Replace with your application code */
    while (1) 
    {
		//Send command
		I2C_MASTER_sendData(I2C_SLAVE_ADDR, PING);
		
		//Send read to slace
		I2C_MASTER_readData(I2C_SLAVE_ADDR);
		
		//Wait for data
		while(!I2C_MASTER_checkData());
		
		//Get data
		uint8_t dataRead = I2C_MASTER_getData();
		
		//Print data
		printf("%d", dataRead);
		
		//Delay 1 second
		_delay_ms(1000);
		
    }
}


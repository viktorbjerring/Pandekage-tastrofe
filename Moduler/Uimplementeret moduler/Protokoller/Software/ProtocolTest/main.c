/*
 * ProtocolTest.c
 *
 * Created: 24/03/2020 15:49:58
 * Author : Anton
 */ 
#define F_CPU 16000000
#define I2C_SLAVE_BUFFER_LENGTH 2

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include "I2C_SLAVE.h"
#include "I2C_MASTER.h"
#include "USART.h"

void wait();

int main(void)
{
	DDRB|= 1<<5 ;
	PORTB = 0 << 5;
	uint8_t c = 0;
	I2C_SLAVE_init();
	I2C_MASTER_init(255,I2C_MASTER_PRESCALE_64);
	I2C_SLAVE_SCL_INT_ENAB(1);
	sei();
	initUART(9600, 8, 1, 0);
	//DDRB = 0x1<<5;
	//sendChar('A');
	
	I2C_MASTER_sendData(0x70, 0x05);
	c = (uint8_t) I2C_SLAVE_getData();
	
    while (1) 
    {
		I2C_SLAVE_sendData(c);
		I2C_MASTER_readData(0x70);
		PORTB = 0 << 5;
		if(c == 255)
		{
			c = 0;
		}
		else
		{
			c++;
		}
		wait();
		sendChar(I2C_MASTER_getData());
		
		
    }
}

#pragma GCC optimize 0
void wait()
{
	while(!I2C_MASTER_checkData())
	{
	}
}
#pragma GCC reset_options
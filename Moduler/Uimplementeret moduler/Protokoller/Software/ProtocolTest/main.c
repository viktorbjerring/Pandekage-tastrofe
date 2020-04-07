/*
 * ProtocolTest.c
 *
 * Created: 24/03/2020 15:49:58
 * Author : Anton
 */ 
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>
//#include "I2C_SLAVE.h"
#include "I2C_MASTER.h"
#include "USART.h"

int main(void)
{
	//I2C_SLAVE_init();
	I2C_MASTER_init(10,I2C_MASTER_PRESCALE_4);
	sei();
	
	
	initUART(9600, 8, 1, 0);
	//DDRB = 0x1<<5;
	//sendChar('A');
    while (1) 
    {
		I2C_MASTER_sendData(0x70,PING);
		_delay_ms(500);
		sendChar('D');
		//TWCR |= (1 << TWEN) | (0 << TWSTO) | (1 << TWSTA) | (1 << TWINT);
		/*while(I2C_SLAVE_checkData())
		{
			sendChar(I2C_SLAVE_getData()+48);
		}*/
    }
}


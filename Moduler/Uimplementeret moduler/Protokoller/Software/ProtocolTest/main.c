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
	uint8_t c = 0x00;
	I2C_SLAVE_init();
	I2C_MASTER_init(255,I2C_MASTER_PRESCALE_64);
	I2C_SLAVE_SCL_INT_ENAB(1);
	sei();
	initUART(9600, 8, 1, 0);
	//DDRB = 0x1<<5;
	//sendChar('A');
    while (1) 
    {
		/*I2C_MASTER_sendData(0x70,GET_FIRST_PAN_STATUS);
	    _delay_ms(500);
		I2C_MASTER_sendData(0x70,BEGIN_COOCKING);
		_delay_ms(500);
		I2C_MASTER_sendData(0x70,MAKE_PANCAKE);
		sendChar(I2C_SLAVE_readFirst());
		sendChar(I2C_SLAVE_readFirst());
		sendChar(I2C_SLAVE_readFirst());*/
		I2C_SLAVE_sendData(c);
		I2C_err_t ret = I2C_MASTER_readData(0x70);
		while(ret != I2C_NOT_EXCLUSIVE){
			if(ret == I2C_READ_FAIL);
			{
				sendChar(0xEE);
			}
			ret = I2C_MASTER_readData(0x70);
			PORTB = 1 << 5;
		}
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
/*
 * I2C_SLAVE_SLAVE.h
 *
 * Created: 27/03/2020 18:32:14
 *  Author: Anton
 */ 


#ifndef I2C_SLAVE_SLAVE_H_
#define I2C_SLAVE_SLAVE_H_

//Sets address if not done by user to avoid compile error.
#ifndef I2C_SLAVE_ADDR
#warning "I2C_SLAVE_ADDR not defined, default address (0x70) used."
#define I2C_SLAVE_ADDR 0x70
#endif

#ifndef I2C_SLAVE_BUFFER_LENGTH
#warning "I2C_SLAVE_BUFFER_LENGTH not defined, buffer may fill all ram."
#define I2C_SLAVE_BUFFER_LENGTH 0
#endif

#include <stdlib.h>
#include <stdbool.h>
#include "I2C_common.h"
#include "USART.h"

// ## Define for port ##
// PORT defines
#define I2C_SLAVE_DDR DDRD
#define I2C_SLAVE_PORT PORTD
#define I2C_SLAVE_PIN PIND
#define I2C_SLAVE_SDA 4
#define I2C_SLAVE_SCL 2

//SCL Interrupt defines
#define I2C_SLAVE_ISC0 ISC00
#define I2C_SLAVE_ISC1 ISC01
#define I2C_SLAVE_INT INT0
#define I2C_SLAVE_SCL_vect INT0_vect

//SDA Interrupt defines;
#define I2C_SLAVE_PCMSK PCMSK2
#define I2C_SLAVE_PCINT PCINT20
#define I2C_SLAVE_PCIE PCIE2
#define I2C_SLAVE_SDA_vect PCINT2_vect

// ## DataContainers ##
char I2C_SLAVE_shouldWrite = 0;
char I2C_SLAVE_recevedAddr = 0;
char I2C_SLAVE_recevedBits = 0;
char I2C_SLAVE_toSend = 0;
char I2C_SLAVE_dataReady = 0;
char I2C_SLAVE_haveSended = 0;


// ## DataReceve ##
struct I2C_SLAVE_DATA{
	char data;
	struct I2C_SLAVE_DATA * ptr;
	};
char I2CC_SLAVE_numberOfReceved = 0;
char I2C_SLAVE_recevedData = 0;
struct I2C_SLAVE_DATA * I2C_SLAVE_first;
struct I2C_SLAVE_DATA * I2C_SLAVE_last;

void I2C_SLAVE_addData(char data)
{
	struct I2C_SLAVE_DATA * temp = (struct I2C_SLAVE_DATA *)malloc(sizeof(struct I2C_SLAVE_DATA));
	I2CC_SLAVE_numberOfReceved++;
	temp->ptr = temp;
	temp->data = data;
	if(I2CC_SLAVE_numberOfReceved == 0)
	{
		I2C_SLAVE_first = temp;
		I2C_SLAVE_last = temp;
	}
	else
	{
		while(I2C_SLAVE_BUFFER_LENGTH != 0 && I2C_SLAVE_BUFFER_LENGTH > I2CC_SLAVE_numberOfReceved-1)
		{
			struct I2C_SLAVE_DATA * temp = I2C_SLAVE_first->ptr;
			free(I2C_SLAVE_first);
			I2C_SLAVE_first = temp;
			I2CC_SLAVE_numberOfReceved--;
		}
		I2C_SLAVE_last->ptr = temp;
		I2C_SLAVE_last = temp;
	}
	I2CC_SLAVE_numberOfReceved++;
}

char I2C_SLAVE_readFirst()
{
	if(I2CC_SLAVE_numberOfReceved != 0)
	{
		char r = I2C_SLAVE_first->data;
		struct I2C_SLAVE_DATA * temp = I2C_SLAVE_first->ptr;
		free(I2C_SLAVE_first);
		I2C_SLAVE_first = temp;
		I2CC_SLAVE_numberOfReceved--;
		return r;
	}
	return 0;
}

// ## Functional macros ##
#define I2C_SLAVE_SET_BIT(toByte,toBit,fromByte,fromBit)  (((toByte) & ~(1 << (toBit))) + ((1 << (toBit))& ((fromByte) >> (fromBit))<<(toBit)))

//SCL interrupt defines.
#define I2C_SLAVE_SCL_FALLING() EICRA = I2C_SLAVE_SET_BIT(EICRA,I2C_SLAVE_ISC1,1,0); EICRA = I2C_SLAVE_SET_BIT(EICRA,I2C_SLAVE_ISC0,0,0)
#define I2C_SLAVE_SCL_RISING() EICRA = I2C_SLAVE_SET_BIT(EICRA,I2C_SLAVE_ISC1,1,0); EICRA = I2C_SLAVE_SET_BIT(EICRA,I2C_SLAVE_ISC0,1,0)
#define I2C_SLAVE_SCL_INT_ENAB(enable) EIMSK = I2C_SLAVE_SET_BIT(EIMSK,I2C_SLAVE_INT,enable,0)

#define I2C_SLAVE_SCL_IS_FALLING() (EICRA & (1<<I2C_SLAVE_ISC1 | 1<<I2C_SLAVE_ISC0)) == (1<<I2C_SLAVE_ISC1)
#define I2C_SLAVE_SCL_IS_RISING() (EICRA & (1<<I2C_SLAVE_ISC1 | 1<<I2C_SLAVE_ISC0)) == (1<<I2C_SLAVE_ISC1 | 1<<I2C_SLAVE_ISC0)

//SDA interrupt defines.
#define I2C_SLAVE_SDA_INT_ENAB(enable) PCICR = I2C_SLAVE_SET_BIT(PCICR,I2C_SLAVE_PCIE,enable,0)
#define I2C_SLAVE_SDA_INT_INIT() PCICR = I2C_SLAVE_SET_BIT(PCICR,I2C_SLAVE_PCIE,1,0);

void I2C_SLAVE_init()
{
	I2C_SLAVE_SCL_RISING();
	I2C_SLAVE_SDA_INT_INIT();
	I2C_SLAVE_SDA_INT_ENAB(1);
	I2C_SLAVE_PORT &= ~(1 << I2C_SLAVE_SDA | 1 << I2C_SLAVE_SCL);
	I2C_SLAVE_DDR |= 1 << I2C_SLAVE_SDA | 1 << I2C_SLAVE_SCL;
}


void I2C_SLAVE_sendBit(char bit)
{
	I2C_SLAVE_toSend = bit << 7;
	I2C_SLAVE_haveSended = 7;
	I2C_SLAVE_dataReady = 1;
}

void I2C_SLAVE_beginSend()
{
	I2C_SLAVE_SCL_FALLING();
	I2C_SLAVE_DDR &= ~(1 << I2C_SLAVE_SDA);
	I2C_SLAVE_dataReady = 0;
	if((I2C_SLAVE_DDR & (1 << I2C_SLAVE_SCL)) == 0)
	{
		I2C_SLAVE_PORT = I2C_SLAVE_SET_BIT(I2C_SLAVE_PORT,I2C_SLAVE_SDA,I2C_SLAVE_toSend,I2C_SLAVE_haveSended);
		I2C_SLAVE_haveSended++;
		I2C_SLAVE_DDR |= 1 << I2C_SLAVE_SCL;
		if(I2C_SLAVE_haveSended > 7)
		{
			I2C_SLAVE_SCL_RISING();
		}
	}
}


int I2C_SLAVE_hold()
{
	I2C_SLAVE_DDR &=  ~(1 << I2C_SLAVE_SCL);
	I2C_SLAVE_PORT &=  ~(1 << I2C_SLAVE_SCL);
	if(I2C_SLAVE_dataReady)
	{
		return 1;
	}
	return 0;
}

ISR(I2C_SLAVE_SCL_vect)
{
	if(I2C_SLAVE_SCL_IS_RISING())
	{
		if(I2C_SLAVE_haveSended > 7)
		{
			I2C_SLAVE_haveSended = 0;
			I2C_SLAVE_PORT &= ~(1 << I2C_SLAVE_SDA);
			I2C_SLAVE_DDR |= 1 << I2C_SLAVE_SDA;
		}
		else if(I2C_SLAVE_recevedBits < 6)
		{
			I2C_SLAVE_recevedAddr = I2C_SLAVE_SET_BIT(I2C_SLAVE_recevedAddr,6-I2C_SLAVE_recevedBits,I2C_SLAVE_PIN,I2C_SLAVE_SDA);
			I2C_SLAVE_recevedBits++;
		}
		else if(I2C_SLAVE_recevedAddr == I2C_SLAVE_ADDR && I2C_SLAVE_recevedBits < 7)
		{
			I2C_SLAVE_shouldWrite = I2C_SLAVE_SET_BIT(I2C_SLAVE_shouldWrite,0,I2C_SLAVE_PIN,I2C_SLAVE_SDA);
			I2C_SLAVE_sendBit(0);
			I2C_SLAVE_beginSend();
			I2C_SLAVE_recevedBits++;
		}
		else if(I2C_SLAVE_recevedAddr == I2C_SLAVE_ADDR && I2C_SLAVE_recevedBits < 15)
		{
			if(!I2C_SLAVE_shouldWrite)
			{
				I2C_SLAVE_recevedData = I2C_SLAVE_SET_BIT(I2C_SLAVE_recevedData,7-(I2C_SLAVE_recevedBits-7),I2C_SLAVE_PIN,I2C_SLAVE_SDA);
				I2C_SLAVE_recevedBits++;
			}
			else if(I2C_SLAVE_shouldWrite && I2C_SLAVE_hold())
			{
				I2C_SLAVE_beginSend();
			}
		}
		else if(I2C_SLAVE_recevedAddr == I2C_SLAVE_ADDR && I2C_SLAVE_recevedBits < 16)
		{
			I2C_SLAVE_addData(I2C_SLAVE_recevedData);
			I2C_SLAVE_recevedBits = 7;
			I2C_SLAVE_SCL_INT_ENAB(0);
			I2C_SLAVE_SDA_INT_ENAB(1);
		}
	}
	else if(I2C_SLAVE_SCL_IS_FALLING())
	{
		I2C_SLAVE_PORT = I2C_SLAVE_SET_BIT(I2C_SLAVE_PORT,I2C_SLAVE_SDA,I2C_SLAVE_toSend,I2C_SLAVE_haveSended);
		I2C_SLAVE_haveSended++;
		if(I2C_SLAVE_haveSended > 7)
		{
			I2C_SLAVE_recevedBits += I2C_SLAVE_haveSended;
			I2C_SLAVE_SCL_RISING();
		}
	}
	
}

ISR(I2C_SLAVE_SDA_vect)
{
	if(((I2C_SLAVE_PIN & 1 << I2C_SLAVE_SCL)))
	{
		if((I2C_SLAVE_PIN & 1 << I2C_SLAVE_SDA) == 0)
		{
			I2C_SLAVE_SCL_INT_ENAB(1);
			sendChar('i');
		}
		else
		{
			I2C_SLAVE_SCL_INT_ENAB(0);
			I2C_SLAVE_recevedAddr = 0;
			I2C_SLAVE_recevedBits = 0;
			sendChar('e');
		}
	}
}

// ## USER COMMANDS Below ##

bool I2C_SLAVE_checkData()
{
	if(I2CC_SLAVE_numberOfReceved)
	{
		return true;
	}
	return false;
}

void I2C_SLAVE_sendData(char data)
{
	I2C_SLAVE_toSend = data;
	I2C_SLAVE_haveSended = 0;
	I2C_SLAVE_dataReady = 1;
}

I2C_commands_t I2C_SLAVE_getData()
{
	return (I2C_commands_t)I2C_SLAVE_readFirst();
}

#endif /* I2C_SLAVE_SLAVE_H_ */
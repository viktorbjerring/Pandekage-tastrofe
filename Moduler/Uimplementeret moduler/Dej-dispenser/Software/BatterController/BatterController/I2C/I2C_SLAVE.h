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
#warning "I2C_SLAVE_BUFFER_LENGTH not defined, default buffer size of 10 used."
#define I2C_SLAVE_BUFFER_LENGTH 10
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "I2C_common.h"

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
volatile char I2C_SLAVE_toSend = 0;
char I2C_SLAVE_tempSave = 0;
char I2C_SLAVE_dataReady = 0;
char I2C_SLAVE_haveSended = 0;
char I2C_SLAVE_recevedData = 0;
bool I2C_SLAVE_startRecived = false;

// ## Functional macros ##
#define I2C_SLAVE_SET_BIT(toByte,toBit,fromByte,fromBit)  (((toByte) & ~(1 << (toBit))) + ((1 << (toBit))& (((fromByte) >> (fromBit))<<(toBit))))

//SCL interrupt defines.
#define I2C_SLAVE_SCL_FALLING() EICRA = I2C_SLAVE_SET_BIT(EICRA,I2C_SLAVE_ISC1,1,0); EICRA = I2C_SLAVE_SET_BIT(EICRA,I2C_SLAVE_ISC0,0,0)
#define I2C_SLAVE_SCL_RISING() EICRA = I2C_SLAVE_SET_BIT(EICRA,I2C_SLAVE_ISC1,1,0); EICRA = I2C_SLAVE_SET_BIT(EICRA,I2C_SLAVE_ISC0,1,0)
#define I2C_SLAVE_SCL_INT_ENAB(enable) EIMSK = I2C_SLAVE_SET_BIT(EIMSK,I2C_SLAVE_INT,enable,0)

#define I2C_SLAVE_SCL_IS_FALLING() ((EICRA & (1<<I2C_SLAVE_ISC1 | 1<<I2C_SLAVE_ISC0)) == (1<<I2C_SLAVE_ISC1))
#define I2C_SLAVE_SCL_IS_RISING() ((EICRA & (1<<I2C_SLAVE_ISC1 | 1<<I2C_SLAVE_ISC0)) == (1<<I2C_SLAVE_ISC1 | 1<<I2C_SLAVE_ISC0))

//SDA interrupt defines.
#define I2C_SLAVE_SDA_INT_ENAB(enable) PCICR = I2C_SLAVE_SET_BIT(PCICR,I2C_SLAVE_PCIE,enable,0)
#define I2C_SLAVE_SDA_INT_INIT() I2C_SLAVE_PCMSK = I2C_SLAVE_SET_BIT(PCICR,I2C_SLAVE_PCINT,1,0);

// ## DataReceve ##
bool I2C_SLAVE_beginHold = false;
unsigned char I2C_SLAVE_first = 0;
unsigned char I2C_SLAVE_last = 0;
bool I2C_SLAVE_full = false;
char I2C_SLAVE_Buffer[I2C_SLAVE_BUFFER_LENGTH];

void I2C_SLAVE_addData(char data);
char I2C_SLAVE_readFirst();

// ## USER COMMANDS Below ##
bool I2C_SLAVE_checkData();
void I2C_SLAVE_sendData(char data);
I2C_commands_t I2C_SLAVE_getData();
void I2C_SLAVE_init();

// ## Internal commands ##
void I2C_SLAVE_sendBit(char bit);
void I2C_SLAVE_beginSend();
int I2C_SLAVE_hold();

// ## Definitions bellow ##

bool I2C_SLAVE_checkData()
{
	if(I2C_SLAVE_first != I2C_SLAVE_last || I2C_SLAVE_full)
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
	
	if(I2C_SLAVE_beginHold)
	{
		I2C_SLAVE_beginHold = false;
		I2C_SLAVE_SDA_INT_ENAB(0);
		I2C_SLAVE_PORT = I2C_SLAVE_SET_BIT(I2C_SLAVE_PORT, I2C_SLAVE_SDA, I2C_SLAVE_toSend, 7);
		I2C_SLAVE_haveSended++;
		while(((I2C_SLAVE_toSend>>7)<<I2C_SLAVE_SDA) != (I2C_SLAVE_PIN & 1 << I2C_SLAVE_SDA)){}
		I2C_SLAVE_SDA_INT_ENAB(1);
	}
	I2C_SLAVE_PORT |= 1 << I2C_SLAVE_SCL;
}

I2C_commands_t I2C_SLAVE_getData()
{
	return (I2C_commands_t)I2C_SLAVE_readFirst();
}

void I2C_SLAVE_init()
{
	I2C_SLAVE_SCL_RISING();
	I2C_SLAVE_SDA_INT_INIT();
	I2C_SLAVE_SDA_INT_ENAB(1);
	I2C_SLAVE_SCL_INT_ENAB(1);
	I2C_SLAVE_DDR &= ~(1 << I2C_SLAVE_SDA | 1 << I2C_SLAVE_SCL);
	I2C_SLAVE_DDR |= 1 << 6 | 1 << 5 | 1 << 7;
	I2C_SLAVE_PORT |= (1 << I2C_SLAVE_SDA | 1 << I2C_SLAVE_SCL);
}

// ## Internal commands ##

void I2C_SLAVE_sendBit(char bit)
{
	if(I2C_SLAVE_dataReady == 1)
	{
		I2C_SLAVE_tempSave = I2C_SLAVE_toSend;
		I2C_SLAVE_dataReady = 2;
	}
	else
	{
		I2C_SLAVE_dataReady = 1;
	}
	I2C_SLAVE_toSend = bit << 7;
	I2C_SLAVE_haveSended = 7;
}

void I2C_SLAVE_beginSend()
{
	I2C_SLAVE_SCL_FALLING();
	if(I2C_SLAVE_dataReady == 2)
		I2C_SLAVE_dataReady = 3;
	else
		I2C_SLAVE_dataReady = 0;
}

int I2C_SLAVE_hold()
{
	//I2C_SLAVE_DDR &=  ~(1 << I2C_SLAVE_SCL);
	
	if(I2C_SLAVE_dataReady == 1 || I2C_SLAVE_dataReady == 2)
	{
		I2C_SLAVE_beginHold = false;
		return 1;
	}
	else
	{
		I2C_SLAVE_PORT &=  ~(1 << I2C_SLAVE_SCL);
	}
	return 0;
}

unsigned char I2C_SLAVE_inc(unsigned char * num)
{
	unsigned char temp = *num;
	(*num)++;
	if(*num >= I2C_SLAVE_BUFFER_LENGTH)
	{
		*num = 0;
	}
	return temp;
}

void I2C_SLAVE_addData(char data)
{
	if(I2C_SLAVE_full)
	{
		I2C_SLAVE_first++;
	}
	I2C_SLAVE_Buffer[I2C_SLAVE_inc(&I2C_SLAVE_last)] = data;
	if(I2C_SLAVE_last == I2C_SLAVE_first)
	{
		I2C_SLAVE_full = true;
	}
}

char I2C_SLAVE_readFirst()
{
	if(I2C_SLAVE_checkData())
	{
		if(I2C_SLAVE_first >= I2C_SLAVE_BUFFER_LENGTH)
		{
			I2C_SLAVE_first = 0;
		}
		if(I2C_SLAVE_full)
		{
			I2C_SLAVE_full = false;
		}
		return I2C_SLAVE_Buffer[I2C_SLAVE_inc(&I2C_SLAVE_first)];
	}
	return 0;
}


ISR(I2C_SLAVE_SDA_vect)
{
	if((I2C_SLAVE_PIN & 1 << I2C_SLAVE_SCL) ==(1 << I2C_SLAVE_SCL))
	{
		if(I2C_SLAVE_PIN & 1 << I2C_SLAVE_SDA)
		{
			I2C_SLAVE_startRecived = false;
		}
		else
		{
			I2C_SLAVE_startRecived = true;
		}
		if(I2C_SLAVE_startRecived == false)
		{
			//sendChar(0xDA);
			I2C_SLAVE_recevedAddr = 0;
			I2C_SLAVE_recevedBits = 0;
			I2C_SLAVE_SCL_RISING();
			I2C_SLAVE_PORT |= (1 << I2C_SLAVE_SDA | 1 << I2C_SLAVE_SCL);
		}
	}
}

ISR(I2C_SLAVE_SCL_vect)
{
	if(I2C_SLAVE_startRecived)
	{
		if(I2C_SLAVE_SCL_IS_RISING())
		{
			if(I2C_SLAVE_haveSended > 7)
			{
				I2C_SLAVE_haveSended = 0;
				I2C_SLAVE_PORT |= (1 << I2C_SLAVE_SDA | 1 << I2C_SLAVE_SCL);
				//I2C_SLAVE_DDR |= 1 << I2C_SLAVE_SDA;
			}
			if(I2C_SLAVE_recevedBits < 7)
			{
				I2C_SLAVE_recevedAddr = I2C_SLAVE_SET_BIT(I2C_SLAVE_recevedAddr,6-I2C_SLAVE_recevedBits,I2C_SLAVE_PIN,I2C_SLAVE_SDA);
				I2C_SLAVE_recevedBits++;
			}
			else if(I2C_SLAVE_recevedAddr == I2C_SLAVE_ADDR && I2C_SLAVE_recevedBits < 8)
			{
				I2C_SLAVE_recevedBits++;
				I2C_SLAVE_shouldWrite = I2C_SLAVE_SET_BIT(I2C_SLAVE_shouldWrite,0,I2C_SLAVE_PIN,I2C_SLAVE_SDA);//0;
				if(I2C_SLAVE_shouldWrite)
				{
					I2C_SLAVE_beginHold = true;
				}
				I2C_SLAVE_sendBit(0);
				I2C_SLAVE_beginSend();
			}
			else if(I2C_SLAVE_recevedAddr == I2C_SLAVE_ADDR && I2C_SLAVE_recevedBits < (9))
			{
				I2C_SLAVE_recevedBits++;
			}
			else if(I2C_SLAVE_recevedAddr == I2C_SLAVE_ADDR && I2C_SLAVE_recevedBits < (17))
			{
				if(!I2C_SLAVE_shouldWrite)
				{
					I2C_SLAVE_recevedData = I2C_SLAVE_SET_BIT(I2C_SLAVE_recevedData,7-(I2C_SLAVE_recevedBits-8),I2C_SLAVE_PIN,I2C_SLAVE_SDA);
					I2C_SLAVE_recevedBits++;
					if(I2C_SLAVE_recevedBits == 16)
					{
						I2C_SLAVE_addData(I2C_SLAVE_recevedData);
						I2C_SLAVE_sendBit(0);
						I2C_SLAVE_beginSend();
						I2C_SLAVE_recevedBits = 8;
					}
				}
				else if(I2C_SLAVE_shouldWrite && I2C_SLAVE_hold())
				{
					I2C_SLAVE_beginSend();
					I2C_SLAVE_recevedBits = 7;
				}
			}
		}
		else if(I2C_SLAVE_SCL_IS_FALLING())
		{
			//sendChar(I2C_SLAVE_haveSended);
			if(I2C_SLAVE_haveSended > 7)
			{
				if(I2C_SLAVE_dataReady == 2 || I2C_SLAVE_dataReady == 3)
				{
					//sendChar(I2C_SLAVE_tempSave);
					I2C_SLAVE_dataReady = 1;
					I2C_SLAVE_haveSended = 1;
					I2C_SLAVE_toSend = I2C_SLAVE_tempSave;
					I2C_SLAVE_PORT = I2C_SLAVE_SET_BIT(I2C_SLAVE_PORT,I2C_SLAVE_SDA,I2C_SLAVE_toSend,7);
				}
				else
				{
					//I2C_SLAVE_recevedBits += I2C_SLAVE_haveSended;
					I2C_SLAVE_PORT |= (1 << I2C_SLAVE_SDA);
				}
				if(I2C_SLAVE_beginHold)
				{
					I2C_SLAVE_hold();
				}
				else
				{
					I2C_SLAVE_SCL_RISING();
				}
			}
			else
			{
				
				I2C_SLAVE_PORT = I2C_SLAVE_SET_BIT(I2C_SLAVE_PORT,I2C_SLAVE_SDA,I2C_SLAVE_toSend,(7-I2C_SLAVE_haveSended));//&= ~(1 << I2C_SLAVE_SDA);
				
				I2C_SLAVE_haveSended++;
			}
		}
	}
	
}




#endif /* I2C_SLAVE_SLAVE_H_ */
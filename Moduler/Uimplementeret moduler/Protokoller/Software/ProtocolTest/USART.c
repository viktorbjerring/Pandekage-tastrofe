/*
 * USART.c
 *
 * Created: 27/11/2018 09.02.19
 *  Author: Anton Vigen Smolarz
 */ 
#include <stdlib.h>
#include "USART.h"
#include "avr/interrupt.h"
#include "util/delay.h"

//HW USART
void initUART(unsigned long baudRate, unsigned char dataBit, unsigned char Rx_int, char Parity)
{
	if((baudRate >= 300) && (baudRate <= 115200) && (dataBit >= 5) && (dataBit <= 8))
	{
		UCSR0B = 0B00011000;
		UCSR0C = (dataBit-5)<<1;
		if(Rx_int)
		{
			UCSR0B |= 0B10000000;
		}
		if (Parity == 'E')
		{
			UCSR0C |= 0B00100000;
		}
		else if(Parity == 'O')
		{
			UCSR0C |= 0B00110000;
		}
		UBRR0 = (16000000+(8*baudRate))/(16*baudRate)-1;
	}
}

unsigned char charReady()
{
	return UCSR0A & (1<<7);
}
char readChar()
{
	while(!(UCSR0A & (1<<7)))
	{}
	return UDR0;
}
void sendChar(char sChar)
{
	while(!(UCSR0A & (1<<5)))
	{}
	UDR0 = sChar;
}
void sendString(char* sString)
{
	while (*sString != 0)
	{
		sendChar(*sString);
		sString++;
	}
}
void sendInteger(int sNum)
{
	char numString[7];
	itoa(sNum,numString,10);
	sendString(numString);
}



//SW UART

#ifdef SW_UART
void SWsendChar(unsigned char sChar)
{
	unsigned char i = 0;
	PORTE &= ~2;
	_delay_us(NO_us);
	
	for(; i < 8; i++)
	{
		PORTE =(PORTE & 0B11111101) + (((sChar >> i) << PINNR) & 0B00000010);
		_delay_us(NO_us);
	}
	PORTE |= 2;
	_delay_us(NO_us);
}

unsigned char SWreadChar()
{
	unsigned char i = 0;
	unsigned char res = 0;
	while(PINE & 1)
	{
		
	}
	_delay_us(NO_us*1.5);
	for(; i < 8; i++)
	{
		res =((PINE & 1) << i) + res;
		_delay_us(NO_us);
	}
	return res;
}

void SWsendString(char* sString)
{
	while (*sString != 0)
	{
		SWsendChar(*sString);
		sString++;
	}
}
void SWsendInteger(int sNum)
{
		char numString[7];
		itoa(sNum,numString,10);
		SWsendString(numString);
}
#endif
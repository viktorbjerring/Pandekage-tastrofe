/************************************************
* "uart.c":                                     *
* Implementation file for Mega2560 UART driver. *
* Using UART 0.                                 *
* Henning Hargaard, 15/11 2016                  *
*************************************************/
#include <avr/io.h>
#include <stdlib.h>
#include "uart.h"

// Constants
#define XTAL 16000000

/*************************************************************************
USART initialization.
  Asynchronous mode.
  RX and TX enabled.
  No interrupts enabled.
  Number of Stop Bits = 1.
  No Parity.
  Baud rate = Parameter.
  Data bits = Parameter.
Parameters:
  BaudRate: Wanted Baud Rate (300-115200).
  Databits: Wanted number of Data Bits (5-8).
  Parity: 'E' (Even parity), 'O' (Odd parity), otherwise No Parity.
*************************************************************************/
void InitUART(unsigned long BaudRate, unsigned char DataBit, char Parity)
{
   if (BaudRate < 300 || BaudRate > 115200 || DataBit < 5 || DataBit > 8)
   {
	   return;
   }
   
   UCSR0A &= 0b11111100;
   UCSR0B = 0b00011000;
   UCSR0C &= 0b00000000;
   
   // Set baud rate
   UBRR0 = XTAL / (16 * BaudRate) - 1;
   
   // Set data bits
   switch(DataBit)
   {
	   case 5: UCSR0C |= 0b00000000; break;
	   case 6: UCSR0C |= 0b00000010; break;
	   case 7: UCSR0C |= 0b00000100; break;
	   case 8: UCSR0C |= 0b00000110; break;
   }
   
   // Set parity
   switch(Parity)
   {
	   case 'E': UCSR0C |= 0b00100000; break;
	   case 'O': UCSR0C |= 0b00110000; break;
	   default: UCSR0C |= 0b00000000; break;
   }
}

/*************************************************************************
Returns 0 (FALSE), if the UART has NOT received a new character.
Returns value <> 0 (TRUE), if the UART HAS received a new character.
*************************************************************************/
unsigned char CharReady()
{
   return UCSR0A & (1<<7);
}

/*************************************************************************
Awaits new character received.
Then this character is returned.
*************************************************************************/
char ReadChar()
{
   while(!CharReady)
   {
	   continue;
   }
   return UDR0;
}

/*************************************************************************
Awaits transmitter register ready.
Then send the character.
Parameter:
  Tegn: Character for sending. 
*************************************************************************/
void SendChar(char Tegn)
{
   while((UCSR0A & (1<<5)) == 0)
   {
	   continue;
   }
   UDR0 = Tegn;
}

/*************************************************************************
Sends 0 terminated string.
Parameter:
  Streng: Pointer to the string. 
*************************************************************************/
void SendString(char* Streng)
{
   while(*Streng != 0)
   {
	   SendChar(*Streng);
	   Streng++;
   }
}

/*************************************************************************
Converts the integer "Tal" to an ASCII string - and then sends this string
using the USART. 
Makes use of the C standard library <stdlib.h>.
Parameter:
  Tal: The integer to be converted and sent. 
*************************************************************************/
void SendInteger(int Tal)
{
   char talString[10];
   itoa(Tal, talString, 10);
   SendString(talString);
}

/************************************************************************/
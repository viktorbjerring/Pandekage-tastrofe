/*
 * I2C_SLAVE_SLAVE.h
 *
 * Created: 27/03/2020 18:32:14
 *  Author: Anton
 */ 


#ifndef I2C_SLAVE_SLAVE_H_
#define I2C_SLAVE_SLAVE_H_



//Sets buffer length if not done by user to avoid compile error.
#ifndef I2C_SLAVE_BUFFER_LENGTH
#warning "I2C_SLAVE_BUFFER_LENGTH not defined, default buffer size of 10 used."
#define I2C_SLAVE_BUFFER_LENGTH 10
#endif

#include <stdlib.h>
#include <stdbool.h>
#include <util/delay.h>
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

//SDA Interrupt defines
#define I2C_SLAVE_PCMSK PCMSK2
#define I2C_SLAVE_PCINT PCINT20
#define I2C_SLAVE_PCIE PCIE2
#define I2C_SLAVE_SDA_vect PCINT2_vect

// ## DataContainers ##
static char I2C_SLAVE_shouldWrite = 0;
static char I2C_SLAVE_recevedAddr = 0;
static char I2C_SLAVE_recevedBits = 0;
static volatile char I2C_SLAVE_toSend = 0;
static char I2C_SLAVE_tempSave = 0;
static char I2C_SLAVE_dataReady = 0;
static char I2C_SLAVE_haveSended = 0;
static char I2C_SLAVE_recevedData = 0;
static bool I2C_SLAVE_startRecived = false;

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

// ## Data Receive Buffer ##
static bool I2C_SLAVE_beginHold = false;
volatile static unsigned char I2C_SLAVE_first = 0;
volatile static unsigned char I2C_SLAVE_last = 0;
volatile static bool I2C_SLAVE_full = false;
static char I2C_SLAVE_Buffer[I2C_SLAVE_BUFFER_LENGTH];

static void I2C_SLAVE_addData(char data);
static char I2C_SLAVE_readFirst();

// #### USER COMMANDS Below ####
volatile bool I2C_SLAVE_checkData(); //Checks if any data is present in the receive data buffer.
void I2C_SLAVE_sendData(char data); //Prepares the sendData buffer for sending. Can both be called before and after master read request.
I2C_commands_t I2C_SLAVE_getData(); //Retrieves data in the data buffer. If the buffer is empty then the function is blocking.
void I2C_SLAVE_init(); //Initializes ports and interrupts for the I2C slave.

// ## Internal commands ##
static void I2C_SLAVE_sendBit(char bit); //Sends single bit. (Used for ack-bit).
static void I2C_SLAVE_beginSend(); //Begins sending data.
static int I2C_SLAVE_hold(); //Holds SCL line until data is ready to be send.

// ## Definitions bellow ##

volatile bool I2C_SLAVE_checkData()
{
	//Checks if all data has been read.
	if(I2C_SLAVE_first != I2C_SLAVE_last || I2C_SLAVE_full)
	{
		return true;
	}
	return false;
}

void I2C_SLAVE_sendData(char data)
{
	//Sets send data to send.
	I2C_SLAVE_toSend = data;
	//Sets number of bits sended to 0.
	I2C_SLAVE_haveSended = 0;
	//Indicates that data is needed to be send.
	I2C_SLAVE_dataReady = 1;
	
	//If begin hold is sat, then sending will begin imidiatly.
	if(I2C_SLAVE_beginHold)
	{
		//Resets begin hold
		I2C_SLAVE_beginHold = false;
		//Disables SDA interrupt to avoid stop bit triggering if toSind most significant is 1.
		I2C_SLAVE_SDA_INT_ENAB(0);
		//Sets the line to most significant bit.
		I2C_SLAVE_PORT = I2C_SLAVE_SET_BIT(I2C_SLAVE_PORT, I2C_SLAVE_SDA, I2C_SLAVE_toSend, 7);
		//Sets that first bit is sended.
		I2C_SLAVE_haveSended++;
		//Waits until SDA reads the correct bit before enabling interrupt agian.
		while(((I2C_SLAVE_toSend>>7)<<I2C_SLAVE_SDA) != (I2C_SLAVE_PIN & 1 << I2C_SLAVE_SDA)){}
		//enables interrupt.
		I2C_SLAVE_SDA_INT_ENAB(1);
	}
	I2C_SLAVE_PORT |= 1 << I2C_SLAVE_SCL;
}

I2C_commands_t I2C_SLAVE_getData()
{
	//Waits for data to have arrived.
	while(!I2C_SLAVE_checkData());
	//Returns first data.
	return (I2C_commands_t)I2C_SLAVE_readFirst();
}

void I2C_SLAVE_init()
{
	//Sets SCL interrupt to rising edge trigger.
	I2C_SLAVE_SCL_RISING();

	//Sets SDA interrupt up.
	I2C_SLAVE_SDA_INT_INIT();
	
	//Enables both interrupts.
	I2C_SLAVE_SDA_INT_ENAB(1);
	I2C_SLAVE_SCL_INT_ENAB(1);
	
	//Sets up ports to inputs with pull up.
	I2C_SLAVE_DDR &= ~(1 << I2C_SLAVE_SDA | 1 << I2C_SLAVE_SCL);
	I2C_SLAVE_DDR |= 1 << 6 | 1 << 5 | 1 << 7;
	I2C_SLAVE_PORT |= (1 << I2C_SLAVE_SDA | 1 << I2C_SLAVE_SCL);
}

// ## Internal commands ##

static void I2C_SLAVE_sendBit(char bit)
{
	//Workaround implementer so ack-bit can be send without the toSend buffer is lost.
	//If data buffer is already updated the toSend buffer is temporarily stored.
	if(I2C_SLAVE_dataReady == 1)
	{
		//Saves  toSend in temporary buffer.
		I2C_SLAVE_tempSave = I2C_SLAVE_toSend;
		//Sets data ready to 2 to indicate that the send buffer should be reverted back to the temporary buffer after sending.
		I2C_SLAVE_dataReady = 2;
	}
	else
	{
		//If the data buffer isn't set, then there is no need to store anything in the temporary buffer.
		I2C_SLAVE_dataReady = 1;
	}
	//Only one bit should be send, therefore bit number 7 is set high and have sended
	//is set to 7, so the byte sending functionality can be utilized for sending only one bit.
	I2C_SLAVE_toSend = bit << 7;
	I2C_SLAVE_haveSended = 7;
}

static void I2C_SLAVE_beginSend()
{
	//sending bits should be updated on a falling edge. SCL int trigger is changed accordingly.
	I2C_SLAVE_SCL_FALLING();
	//If the temporary buffer is used dataReady is set to 3, to indicate that sending has begun and
	//that the buffer should be updated.
	if(I2C_SLAVE_dataReady == 2)
		I2C_SLAVE_dataReady = 3;
	else
	//If the temporary buffer isn't used, then it is reverted to show that there is no new data in the toSend buffer. 
		I2C_SLAVE_dataReady = 0;
}

static int I2C_SLAVE_hold()
{
	//If data is ready then it sets beginHold false and returns true.
	if(I2C_SLAVE_dataReady == 1 || I2C_SLAVE_dataReady == 2)
	{
		I2C_SLAVE_beginHold = false;
		return 1;
	}
	else
	{
		//If no data is ready, then SCL is pulled low.
		I2C_SLAVE_PORT &=  ~(1 << I2C_SLAVE_SCL);
	}
	//Returns false if no data is ready.
	return 0;
}

static unsigned char I2C_SLAVE_inc(volatile unsigned char * num)
{
	//Save return value.
	unsigned char temp = *num;

	//Incrementing number 
	(*num)++;
	
	//If number is higher than receive buffer length then its reset to 0.
	if(*num >= I2C_SLAVE_BUFFER_LENGTH)
	{
		*num = 0;
	}
	return temp;
}

static void I2C_SLAVE_addData(char data)
{
	//If the buffer is full, then the oldest data will be overwritten moving first data one.
	if(I2C_SLAVE_full)
	{
		I2C_SLAVE_inc(&I2C_SLAVE_first);
	}
	//Sets last data and increments last. (Hence last will always be the length of the data in the buffer)
	I2C_SLAVE_Buffer[I2C_SLAVE_inc(&I2C_SLAVE_last)] = data;
	//If last position is the same as first the buffer is either full or empty. Therefor at boolean is sat if its full.
	if(I2C_SLAVE_last == I2C_SLAVE_first)
	{
		I2C_SLAVE_full = true;
	}
}

static char I2C_SLAVE_readFirst()
{
	//If buffer is not empty then it will read the buffer.
	if(I2C_SLAVE_checkData())
	{
		//After the first is read, then the buffer isn't full anymore.
		if(I2C_SLAVE_full)
		{
			I2C_SLAVE_full = false;
		}
		//Returns the buffer en increments first by 1.
		return I2C_SLAVE_Buffer[I2C_SLAVE_inc(&I2C_SLAVE_first)];
	}
	return 0;
}

//SCL interrupt for handling data transfer.
ISR(I2C_SLAVE_SCL_vect)
{
	//If start-bit received:
	if(I2C_SLAVE_startRecived)
	{
		//If SCL is triggering on rising edge i.e. it's currently reading from SDA.
		if(I2C_SLAVE_SCL_IS_RISING())
		{
			//If it just have been writing to SDA reset pull up to high.
			if(I2C_SLAVE_haveSended > 7)
			{
				I2C_SLAVE_haveSended = 0;
				I2C_SLAVE_PORT |= (1 << I2C_SLAVE_SDA | 1 << I2C_SLAVE_SCL);
			}
			//First 7 bits read Address.
			if(I2C_SLAVE_recevedBits < 7)
			{
				I2C_SLAVE_recevedAddr = I2C_SLAVE_SET_BIT(I2C_SLAVE_recevedAddr,6-I2C_SLAVE_recevedBits,I2C_SLAVE_PIN,I2C_SLAVE_SDA);
				I2C_SLAVE_recevedBits++;
			}
			//If address matching:
			else if(I2C_SLAVE_recevedAddr == I2C_SLAVE_ADDR && I2C_SLAVE_recevedBits < 8)
			{
				
				I2C_SLAVE_recevedBits++;
				//Read the readWrite bit.
				I2C_SLAVE_shouldWrite = I2C_SLAVE_SET_BIT(I2C_SLAVE_shouldWrite,0,I2C_SLAVE_PIN,I2C_SLAVE_SDA);
				//If it should write, begin hold is set to true.
				if(I2C_SLAVE_shouldWrite)
				{
					//This indicates that the line should be hold right after ack i send.
					I2C_SLAVE_beginHold = true;
				}
				//Set buffer to ack-bit and begin sending.
				I2C_SLAVE_sendBit(0);
				I2C_SLAVE_beginSend();
			}
			else if(I2C_SLAVE_recevedAddr == I2C_SLAVE_ADDR && I2C_SLAVE_recevedBits < (17))
			{
				//If read from master after ack-send if I2C_SLAVE_shouldWrite is false.
				I2C_SLAVE_recevedData = I2C_SLAVE_SET_BIT(I2C_SLAVE_recevedData,7-(I2C_SLAVE_recevedBits-8),I2C_SLAVE_PIN,I2C_SLAVE_SDA);
				I2C_SLAVE_recevedBits++;
				if(I2C_SLAVE_recevedBits == 16)
				{
					//When all bits send add data and ack.
					I2C_SLAVE_addData(I2C_SLAVE_recevedData);
					I2C_SLAVE_sendBit(0);
					I2C_SLAVE_beginSend();
					//Return to bit 8 to continue reading. (Only reading more bytes in one header is not implemented yet.)
					I2C_SLAVE_recevedBits = 8;
				}
			}
		}
		//If SCL is triggering on falling edge i.e. it's currently writing on SDA.
		else if(I2C_SLAVE_SCL_IS_FALLING())
		{
			//Checks if all data is send.
			if(I2C_SLAVE_haveSended > 7)
			{
				//Updates buffer with temporary buffer if I2C_SLAVE_dataReady is 2 or 3.
				if(I2C_SLAVE_dataReady == 2 || I2C_SLAVE_dataReady == 3)
				{
					I2C_SLAVE_dataReady = 1;
					I2C_SLAVE_haveSended = 1;
					I2C_SLAVE_toSend = I2C_SLAVE_tempSave;
					I2C_SLAVE_PORT = I2C_SLAVE_SET_BIT(I2C_SLAVE_PORT,I2C_SLAVE_SDA,I2C_SLAVE_toSend,7);
				}
				else
				{
					//Frees SDA if done sending.
					I2C_SLAVE_PORT |= (1 << I2C_SLAVE_SDA);
				}
				if(I2C_SLAVE_beginHold)
				{
					//Holds the line, if more data is to be sended.
					I2C_SLAVE_hold();
				}
				else
				{
					//More work here needs to be done in order to be able to write multiple bytes.
					//Returns the line to reading
					I2C_SLAVE_SCL_RISING();
					//Makes sure nothing is read to the buffer.
					I2C_SLAVE_recevedBits = 100;
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

//SDA interrupt for handling stop and start bit.
ISR(I2C_SLAVE_SDA_vect)
{
	//Both stop and start bit is triggered while SCL is high.
	if((I2C_SLAVE_PIN & (1 << I2C_SLAVE_SCL)) ==(1 << I2C_SLAVE_SCL))
	{
		//If SDA is high then the interrupt was triggered on a rising edge and therefore it's a stop-bit.
		if(I2C_SLAVE_PIN & (1 << I2C_SLAVE_SDA))
		{
			I2C_SLAVE_startRecived = false;
		}
		//Else it is a start-bit.
		else
		{
			I2C_SLAVE_startRecived = true;
		}
		//If it was a stop-bit reset received data, and set SCL trigger to rising.
		if(I2C_SLAVE_startRecived == false)
		{
			I2C_SLAVE_recevedAddr = 0;
			I2C_SLAVE_recevedBits = 0;
			I2C_SLAVE_SCL_RISING();
		}
	}
}

#endif /* I2C_SLAVE_SLAVE_H_ */
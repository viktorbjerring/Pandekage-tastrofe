#ifndef I2C_MASTER_HEADER
#define I2C_MASTER_HEADER

#include "I2C_common.h"
#include <stdbool.h>

#define I2C_write	0
#define I2C_read	1

#include <avr/interrupt.h>

//Prescale values - 1, 4, 16 or 64
typedef enum {
	I2C_MASTER_PRESCALE_1 = 0,
	I2C_MASTER_PRESCALE_4 = 1,
	I2C_MASTER_PRESCALE_16 = 2,
	I2C_MASTER_PRESCALE_64 = 3,
} I2C_MASTER_prescale_t;

I2C_err_t I2C_MASTER_sendData(char addr, I2C_commands_t cmd);		//Write kommando
I2C_err_t I2C_MASTER_readData(char addr);							//Read fra slave
volatile bool I2C_MASTER_checkData();										//Check for new data
char I2C_MASTER_getData();											//Read the data
void I2C_MASTER_init(char bitRate, I2C_MASTER_prescale_t pre);		// 1MHz/(2*bitRate * pre)

static volatile bool I2C_MASTER_hw_mutex = false;
static volatile uint8_t local_read_data = 0;
static volatile bool data_ready_flag = false;

void I2C_MASTER_init(char bitRate, I2C_MASTER_prescale_t pre)
{
	TWBR = bitRate;	//Set bitRate
	TWSR = (char)pre;	//Set prescaler
	
	TWCR = (1 << TWEN);	//Setup TWI to send NACK when data is recieved and enable HW
}

I2C_err_t I2C_MASTER_sendData(char addr, I2C_commands_t cmd)
{

	//Check and lock mutex
	if (I2C_MASTER_hw_mutex)
	return I2C_NOT_EXCLUSIVE;
	
	I2C_MASTER_hw_mutex = true;
	
	//Start by sending a start condition
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);

	//Wait for start to transmit
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x08)
	goto I2C_write_fail;
	
	//Insert slave ADDR and write bit to data register
	TWDR = (addr << 1) + I2C_write;
	
	//Send addr and W
	TWCR = (1 << TWEN) | (1 << TWINT);

	//Wait for status
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) == 0x20)
	goto I2C_write_fail;
	
	//Write data
	TWDR = cmd;
	TWCR = (1 << TWEN) | (1 << TWINT);
	
	//Wait for status
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) == 0x30)
	goto I2C_write_fail;
	
	//Send stop
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
	
	//Wait for stop-bit send
	while(TWCR & (1 << TWSTO));
	//Unlock mutex
	I2C_MASTER_hw_mutex = false;
	return I2C_OK;
	
	
	I2C_write_fail:
	//Send stop and stop interrupts
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);

	//Wait for stop-bit send
	while(TWCR & (1 << TWSTO));
	
	//Reset I2C module
	TWCR &= ~(1 << TWEN);

	I2C_MASTER_hw_mutex = false;
	return 	I2C_SEND_FAIL;
}

I2C_err_t I2C_MASTER_readData(char addr) {

	//Lock mutex
	if (I2C_MASTER_hw_mutex)
	return I2C_NOT_EXCLUSIVE;
	
	I2C_MASTER_hw_mutex = true;

	//Start by sending a start condition
	TWCR = (1 << TWEN) | (1 << TWSTA) | (1 << TWINT);
	
	//Wait for start to transmit
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x08)
	goto I2C_read_fail;
	
	
	//Insert slave ADDR and read bit to data register
	TWDR = (addr << 1) + I2C_read;
	
	//Send addr and R
	TWCR = (1 << TWEN) | (1 << TWINT);
	
	//Wait for status
	while (!(TWCR & (1 << TWINT))){}
	if ((TWSR & 0xF8) == 0x48)
	goto I2C_read_fail;
	//Enable read interrupt, so CPU can do other stuff
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
	
	return I2C_OK;
	//Do NOT unlock mutex before data is read
	
	I2C_read_fail:
	//Send stop and stop interrupts
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);

	//Wait for stop-bit send
	while(TWCR & (1 << TWSTO));

	//Reset I2C module
	TWCR &= ~(1 << TWEN);

	I2C_MASTER_hw_mutex = false;
	return I2C_READ_FAIL;
}

volatile bool I2C_MASTER_checkData() {
	return data_ready_flag;
}

char I2C_MASTER_getData() {
	
	//Block if no data
	while(!I2C_MASTER_checkData());
	
	//Update status
	data_ready_flag = false;
	
	//Return data
	return local_read_data;
}

//Read data from TWI
ISR(TWI_vect) {

	//Save data
	local_read_data = TWDR;
	
	//Set flag
	data_ready_flag = true;
	
	//Send stop and stop interrupts
	TWCR = (1 << TWEN) | (1 << TWSTO) | (1 << TWINT);
	
	//Enable interrupt nesting
	sei();
	
	//Wait for stop-bit send
	while(TWCR & (1 << TWSTO));

	//Unlock mutex from read
	I2C_MASTER_hw_mutex = false;
	
}

#endif
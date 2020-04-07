/*
 * I2C_MASTER.c
 *
 * Created: 31/03/2020 17.27.02
 *  Author: Mikkel
 */ 

#include "I2C_MASTER.h"
#include "USART.h"

#define I2C_write	0
#define I2C_read	1

#include <avr/interrupt.h>

static bool I2C_MASTER_hw_mutex = false;
static uint8_t local_read_data = 0;
static bool data_ready_flag = false;

void I2C_MASTER_init(char bitRate, I2C_MASTER_prescale_t pre) 
{	
	TWBR = bitRate;	//Set bitRate
	TWSR = ((pre & 0x01) << TWPS0) | ((pre & 0x02) << TWPS1);	//Set prescaler
	
	TWCR = (0 << TWEA) | (1 << TWEN);	//Setup TWI to send NACK when data is recieved and enable HW
}

I2C_err_t I2C_MASTER_sendData(char addr, I2C_commands_t cmd) 
{	

	//Check and lock mutex
	if (I2C_MASTER_hw_mutex)
		return I2C_NOT_EXCLUSIVE;
	
	I2C_MASTER_hw_mutex = true;
	
	//Start by sending a start condition
	TWCR = (1 << TWEN) | (0 << TWSTO) | (1 << TWSTA) | (1 << TWINT);

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
	if ((TWSR & 0xF8) == 0x38) 
		goto I2C_write_fail;
			
	//Write data
	TWDR = cmd;
	TWCR = (1 << TWEN) | (1 << TWINT);
	
	//Wait for status
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) == 0x38)
		goto I2C_write_fail;
	
	//Send stop	
	TWCR = (1 << TWEN) | (1 << TWSTO) | (0 << TWSTA) | (1 << TWINT);
	
	//Unlock mutex	
	I2C_MASTER_hw_mutex = false;
	return I2C_OK;
	
	
I2C_write_fail:
	I2C_MASTER_hw_mutex = false;
	return 	I2C_SEND_FAIL;
}

I2C_err_t I2C_MASTER_readData(char addr) {

	//Lock mutex
	if (I2C_MASTER_hw_mutex)
		return I2C_NOT_EXCLUSIVE;
		
	I2C_MASTER_hw_mutex = true;

	//Start by sending a start condition
	TWCR = (1 << TWEN) | (0 << TWSTO) | (1 << TWSTA) | (1 << TWINT);
	
	//Wait for start to transmit
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != 0x08)
		goto I2C_read_fail;
	
	
	//Insert slave ADDR and read bit to data register
	TWDR = (addr << 1) + I2C_read;
	
	//Send addr and R
	TWCR = (1 << TWEN) | (1 << TWINT);
	
	//Wait for status
	while (!(TWCR & (1 << TWINT)));
	if ((TWSR & 0xF8) == 0x38)
		goto I2C_read_fail;
		
	//Enable read interrupt, so CPU can do other stuff
	TWCR = (1 << TWEN) | (1 << TWIE) | (1 << TWINT);
	
	return I2C_OK;
	//Do NOT unlock mutex before data is read
	
I2C_read_fail:
	I2C_MASTER_hw_mutex = false;
	return I2C_READ_FAIL;
}

bool I2C_MASTER_checkData() {
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
	TWCR = (1 << TWEN) | (0 << TWIE) | (1 << TWSTO) | (0 << TWSTA) | (1 << TWINT);
	
	//Unlock mutex from read
	I2C_MASTER_hw_mutex = false;
	
}
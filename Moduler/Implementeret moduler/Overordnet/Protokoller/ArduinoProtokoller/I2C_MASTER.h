#ifndef I2C_MASTER_HEADER
#define I2C_MASTER_HEADER

#include "I2C_common.h"
#include <stdbool.h>

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

#endif
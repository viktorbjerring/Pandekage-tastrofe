/*
	This is the .h file for the implementation of the TWI HW in slave mode for the ATmega328p.
	This implementation is part of the project PRJ4 - Pan-De-Mix and is developed as part of education on AU.
	
	Created: 24/03/2020
	By:	Mikkel Kirkegaard
	License: GNU
*/

#ifndef PAN_DE_MIX_I2C_HW
#define PAN_DE_MIX_I2C_HW

#include <avr/interrupt.h>

#define SLAVE_I2C_ADDR		0x01	//MAX 127 (0x7F)

typedef enum {
	PING = 0,				//CHECK/DEBUG
	GET_FIRST_PAN_STATUS,	//BATTER - PAN
	BEGIN_COOCKING,			//BATTER - PAN
	MAKE_PANCAKE,			//CNTRL - BATTER
	
} I2C_commands_t;

typedef enum {
	ALL_OK = 0,
	NACK,
	DATA_LOST,
	
} ret_err_t;

typedef enum {
	I2C_HW_MASTER_MODE = 0,
	I2C_HW_SLAVE_MODE,
} I2C_HW_mode_t;

ret_err_t I2C_HW_init (I2C_HW_mode_t I2C_mode);

ret_err_t I2C_send_command(uint8_t slave_addr, I2C_commands_t data);

ret_err_t I2C_send_response(uint8_t * data, uint16_t len);

ISR(TWI, ISR_NOBLOCK);

#endif //PAN_DE_MIX_I2C_SLAVE





#ifndef I2C_COMMON_HEADER
#define I2C_COMMON_HEADER

#include <avr/io.h>
#include <avr/interrupt.h>

typedef enum {
	PING = 0,				//CHECK/DEBUG
	GET_FIRST_PAN_STATUS,	//BATTER - PAN
	BEGIN_COOCKING,			//BATTER - PAN
	MAKE_PANCAKE,			//CNTRL - BATTER
	GET_BATTER_LEVEL,		//CNTRL - BATTER
	TURN_ON_COOLING,		//CNTRL - BATTER
	
} I2C_commands_t;

#endif
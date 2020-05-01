#ifndef I2C_COMMON_HEADER
#define I2C_COMMON_HEADER

#define I2C_SLAVE_ADDR	0x70

typedef enum {
	PING = 0,					//CHECK/DEBUG
	GET_FIRST_PAN_STATUS = 1,	//BATTER - PAN
	BEGIN_COOKING = 2,			//BATTER - PAN
	MAKE_PANCAKE = 3,			//CNTRL - BATTER
	GET_BATTER_AMOUNT = 4,		//CNTRL - BATTER
	TURN_ON_COOLING = 5,		//CNTRL - BATTER
	TURN_OFF_COOLING = 6,		//CNTRL - BATTER
	TURN_OFF_ALARM = 7,			//CNTRL - BATTER
} I2C_commands_t;

typedef enum {
	I2C_OK = 0,				//All OK
	I2C_SEND_FAIL,			//Fail in send
	I2C_READ_FAIL,			//Fail in read
	I2C_NOT_EXCLUSIVE,		//Mutex is taken!
} I2C_err_t;

#endif
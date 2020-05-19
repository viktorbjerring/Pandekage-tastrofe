#ifndef I2C_COMMON_HEADER
#define I2C_COMMON_HEADER

//Sets address if not done by user to avoid compile error.
#ifndef I2C_SLAVE_ADDR
#warning "I2C_SLAVE_ADDR not defined, default address (0x70) used."
#define I2C_SLAVE_ADDR 0x70
#endif

typedef enum {
	PING = 0,				//CHECK/DEBUG
	GET_FIRST_PAN_STATUS,	//BATTER - PAN
	BEGIN_COOKING,			//BATTER - PAN
	MAKE_PANCAKE,			//CNTRL - BATTER
	GET_BATTER_LEVEL,		//CNTRL - BATTER
	TURN_ON_COOLING,		//CNTRL - BATTER
	TURN_OFF_COOLING,		//CNTRL - BATTER
	TURN_OFF_ALARM,			//CNTRL - BATTER
} I2C_commands_t;

typedef enum {
	I2C_OK = 0,				//All OK
	I2C_SEND_FAIL,			//Fail in send
	I2C_READ_FAIL,			//Fail in read
	I2C_NOT_EXCLUSIVE,		//Mutex is taken!
} I2C_err_t;

#endif
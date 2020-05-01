#ifndef I2C_COMMON_HEADER
#define I2C_COMMON_HEADER
#define ERROR 255 // 255 er fejlkode.
typedef enum {
	PING = 0,				//CHECK/DEBUG
	GET_FIRST_PAN_STATUS = 1,	//BATTER - PAN
	BEGIN_COOCKING = 2,			//BATTER - PAN
	MAKE_PANCAKE = 3,			//CNTRL - BATTER
	GET_BATTER_LEVEL = 4,		//CNTRL - BATTER
	TURN_ON_COOLING = 5,
	TURN_OFF_COOLING = 6,		//CNTRL - BATTER
	CLEAR_BATTER_ALARM = 7
} I2C_commands_t;

typedef enum {
	I2C_OK = 0,				//All OK
	I2C_SEND_FAIL = 1,			//Fail in send
	I2C_READ_FAIL = 2,			//Fail in read
	I2C_NOT_EXCLUSIVE = 3,		//Mutex is taken!
} I2C_err_t;

#endif

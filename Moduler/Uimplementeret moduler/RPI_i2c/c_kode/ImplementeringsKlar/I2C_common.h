#ifndef I2C_COMMON_HEADER
#define I2C_COMMON_HEADER
#define ERROR 255 // 255 er fejlkode.
typedef enum {
	PING = 0,				//CHECK/DEBUG
	GET_FIRST_PAN_STATUS,	//BATTER - PAN
	BEGIN_COOCKING,			//BATTER - PAN
	MAKE_PANCAKE,			//CNTRL - BATTER
	GET_BATTER_LEVEL,		//CNTRL - BATTER
	TURN_ON_COOLING,		//CNTRL - BATTER
	
} I2C_commands_t;

typedef enum {
	I2C_OK = 0,				//All OK
	I2C_SEND_FAIL,			//Fail in send
	I2C_READ_FAIL,			//Fail in read
	I2C_NOT_EXCLUSIVE,		//Mutex is taken!
} I2C_err_t;

#endif
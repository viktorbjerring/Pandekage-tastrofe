#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "I2C_Master.h"

#define ADDR        0x70
#define STD_CMD     PING

int main(int argc, char* argv[])
{
    I2C_commands_t cmd = (argc < 2? STD_CMD : atoi(argv[1]));        
	I2C_MASTER_init();
    printf("Writing command %x to %d\n", cmd, ADDR);
	uint8_t temp;
    switch (cmd)
    {
    case MAKE_PANCAKE :     // ingen returdata.
    case TURN_ON_COOLING :
    case TURN_OFF_COOLING :
    case CLEAR_BATTER_ALARM :
        I2C_MASTER_sendData(ADDR,cmd);
		temp = 1;
		break;

    case GET_BATTER_LEVEL : // rerunerer batter Level.
        while (I2C_OK!=I2C_MASTER_sendData(ADDR,cmd));
        while (I2C_OK!=I2C_MASTER_readData(ADDR));
        //I2C_MASTER_checkData();
		temp = I2C_MASTER_getdata();
		break;
		

    default:
        return(ERROR); // har værdien 255
    }
	I2C_MASTER_close();
	return(temp);
}

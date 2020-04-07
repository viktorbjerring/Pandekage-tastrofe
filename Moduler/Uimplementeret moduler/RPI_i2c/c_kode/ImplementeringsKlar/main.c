#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "I2C_Master.h"

#define STD_ADDR    0x48
#define STD_CMD     PING

int main(int argc, char* argv[])
{

    char addr = (argc < 2? STD_ADDR : atoi(argv[1]));
    I2C_commands_t cmd = (argc < 3? STD_CMD : atoi(argv[2]));        

    I2C_MASTER_init();

    printf("Wrinting command %x to %d\n", cmd, addr);
    I2C_MASTER_sendData(addr,PING);

    printf("Reading data from addr %d\n", addr);
    I2C_MASTER_checkData();
    I2C_MASTER_readData(addr);
    printf("Data read from addr %d is %x\n", addr, I2C_MASTER_getdata());

}
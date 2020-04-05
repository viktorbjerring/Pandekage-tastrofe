#include <stdio.h>
#include <unistd.h>
#include "I2C_Master.h"
#define addr 0x48
int main(void)
{
    I2C_MASTER_init();
    printf("after init\n");
    I2C_MASTER_sendData(addr,PING);
    printf("Check 1 %d\n",I2C_MASTER_checkData());
    I2C_MASTER_readData(addr);
    printf("Get data %d\n",I2C_MASTER_getdata());
    printf("Check 2 %d\n",I2C_MASTER_checkData());
    I2C_MASTER_close();
    printf("closed\n");
}

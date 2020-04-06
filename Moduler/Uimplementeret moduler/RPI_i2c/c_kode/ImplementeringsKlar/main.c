#include <stdio.h>
#include <unistd.h>
#include "I2C_Master.h"
int main(void)
{
    I2C_MASTER_init();
    I2C_MASTER_sendData(Adress1,PING);
    I2C_MASTER_checkData();
    I2C_MASTER_readData(Adress1);
    I2C_MASTER_getdata();
    I2C_MASTER_checkData();
    I2C_MASTER_sendData(Adress2,PING);
    I2C_MASTER_checkData();
    I2C_MASTER_readData(Adress2);
    I2C_MASTER_getdata();
    I2C_MASTER_close();
}
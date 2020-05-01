#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include "I2C_Master.h"
#include <linux/i2c-dev.h>
#include <unistd.h>

static int I2CFile;

static bool data_ready_flag = false;
static char buf = 1;

#define I2C_COMMAND_LENGTH  1

I2C_err_t I2C_MASTER_sendData(char addr, I2C_commands_t cmd)
{
    //Set slave addr
    while (ioctl(I2CFile, I2C_SLAVE, addr) < 0);

    //Write command to slave
    while (write(I2CFile,(char*) & cmd, I2C_COMMAND_LENGTH) != I2C_COMMAND_LENGTH);

    return I2C_OK;
}

I2C_err_t I2C_MASTER_readData(char addr)
{
    //Set slave addr
    while (ioctl(I2CFile, I2C_SLAVE, addr) < 0);

    //Read I2C data
    while (read(I2CFile, &buf, I2C_COMMAND_LENGTH) < I2C_COMMAND_LENGTH);
    printf("%x\n",buf);
    //Set read flag
    data_ready_flag = true;
    return I2C_OK;
}

bool I2C_MASTER_checkData()
{
    return(data_ready_flag);
}

char I2C_MASTER_getdata()
{

    while(!I2C_MASTER_checkData());
    data_ready_flag = false;
    return(buf);
}

void I2C_MASTER_init() 
{
    do
    {
        I2CFile = open("/dev/i2c-1", O_RDWR);
    } while (I2CFile == -1);

}

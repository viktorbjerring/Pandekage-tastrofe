#include <fcntl.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include "I2C_Master.h"
#include <linux/i2c-dev.h>
#include <unistd.h>



char flag=false;
char buf=1;
void I2C_MASTER_sendData(char addr, enum I2C_commands_t cmd)
{
    int err;
    do
    {
        err = ioctl(I2CFile, I2C_SLAVE, addr);
        printf("%d,\n",err);
    } while (err<0);
    do
    {
        err=write(I2CFile,(char*)&cmd,1);
        printf("%d,\n",err);
    } while (err!=1);
}

void I2C_MASTER_readData(char addr)
{

    int err;
    do
    {
        err=ioctl(I2CFile, I2C_SLAVE, addr);
        printf("%d,\n",err);
    } while (err<0);
    do
    {
        
        err = read(I2CFile, &buf, 1);
        printf("%d,\n",err);
    } while (err<1);
    flag=true;

}

bool I2C_MASTER_checkData()
{
    return(flag);
}

char I2C_MASTER_getdata()
{

    while(!I2C_MASTER_checkData());
    flag=false;
    return(buf);
}

void I2C_MASTER_init()
{
    do
    {
        I2CFile = open("/dev/i2c-1", O_RDWR);
    } while (I2CFile==-1);

}

void I2C_MASTER_close()
{
    close(I2CFile);
}

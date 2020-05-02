
#ifndef I2C_MASTER_HEADER
#define I2C_MASTER_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include "I2C_common.h"

//NOT USED ON RPI
typedef enum {  
    I2C_MASTER_PRESCALE_1 = 0,
    I2C_MASTER_PRESCALE_4 = 1,
    I2C_MASTER_PRESCALE_16 = 2,
    I2C_MASTER_PRESCALE_64 = 3,
} I2C_MASTER_prescaler_t;

I2C_err_t I2C_MASTER_sendData(char addr, I2C_commands_t cmd);

I2C_err_t I2C_MASTER_readData(char addr);

bool I2C_MASTER_checkData();

uint8_t I2C_MASTER_getdata();

void I2C_MASTER_init();

void I2C_MASTER_close();

#ifdef __cplusplus
}
#endif

#endif

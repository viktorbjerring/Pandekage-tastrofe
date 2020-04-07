#include <stdbool.h>
#define Adress1 0x20 // skal ændres til dens reelle adresse.
#define Adress2 0x40 // skal ændres til dens reelle adresse.
int I2CFile;

enum I2C_commands_t{PING, GET_FIRST_PAN_STATUS, BEGIN_COOKING, MAKE_PANCAKE, GET_BATTER_LEVEL, TURN_ON_COOLING};

enum I2C_MASTER_prescaler_t{I2C_MASTER_PRESCALE_1, I2C_MASTER_PRESCALE_4, I2C_MASTER_PRESCALE_16, I2C_MASTER_PRESCALE_64};

void I2C_MASTER_sendData(char addr, enum I2C_commands_t cmd);

void I2C_MASTER_readData(char addr);

bool I2C_MASTER_checkData();

char I2C_MASTER_getdata();

void I2C_MASTER_init();

void I2C_MASTER_close();
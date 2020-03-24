#include "i2C_slave.h"
#include <util/twi.h>

void send_start();

int I2C_HW_init (I2C_HW_mode_t I2C_mode) {
	
	sei();		//Enable interrupts
	
	if (I2C_mode == I2C_HW_SLAVE_MODE) {
		
		TWAR = SLAVE_I2C_ADDR << 1;	//Setup slave addr
		TWCR.TWEN = 1;				//Start TWI
	}
}

int I2C_send_command(uint8_t slave_addr, I2C_commands_t data){
	//TBD
	
	//send_start();
	//
	//while (TWSR !=	TW_MT_DATA_ACK);
	//
	//TWDR = (slave_addr << 1) + TW_WRITE;
	//
	//if (TWSR == TW_MT_SLA_NACK)
		//return 
	
}

int I2C_send_response(uint8_t * data, uint16_t len) {
	
}

void send_start() {
	TWCR = 0xA4;	
}

ISR(TWI, ISR_NOBLOCK) {		//LOW priority interrupt
	
}
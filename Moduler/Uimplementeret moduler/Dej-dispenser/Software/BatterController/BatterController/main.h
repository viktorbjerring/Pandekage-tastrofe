/*
 * main.h
 *
 * Created: 19/04/2020 17:57:42
 *  Author: oskar
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define INDCTR_DDR      DDRB
#define INDCTR_PORT     PORTB
#define INDCTR_PORT_NUM PORTB0

#define MIN_BATTER_LEVEL 5

// Main function
int main();

// UC2
void pancakeBegin();

// UC3
int getBatterAmount();

// UC4
void turnOnCooling();

#endif /* MAIN_H_ */
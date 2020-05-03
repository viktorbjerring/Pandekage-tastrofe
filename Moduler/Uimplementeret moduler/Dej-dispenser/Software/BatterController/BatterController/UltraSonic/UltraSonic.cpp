/*
 * UltraSonic.cpp
 *
 * Created: 06/04/2020 12.13.46
 *  Author: au615804
 */ 


#include "UltraSonic.h"

volatile uint16_t batterLevel_ = 0;
bool isEcho_ = false;

ISR(PCINT1_vect) {
	/* If currently timing PW */
	if (isEcho_)
	{
		/* Turn off timer2 */
		ULTRSNC_TCCRB = 0;
		
		/* Calculate and save distance */
		batterLevel_ = static_cast<uint16_t>(10*REGRESSION(ULTRSNC_TCNT)); // Distance in mm
		
		/* Calculate batter level from bottom of container */
		batterLevel_ = CNTNR_H - batterLevel_;
		
		/* Turn off pin change 13 interrupt */
		ULTRSNC_PCMSK &= ~(1 << ULTRSNC_PCINT);
	}
	else
	{
		/* Reset and start timer2 */
		ULTRSNC_TCNT = 0;
		ULTRSNC_TCCRB = 0b00000110;
		
		isEcho_ = true;
	}
}

void initUltrasonic() {
	ULTRSNC_TRGGR_DDR |=  (1<<ULTRSNC_TRGGR_PORT_NUM); // Trigger pin
	ULTRSNC_ECHO_DDR &= ~(1<<ULTRSNC_ECHO_PIN_NUM); // Echo pin
	ULTRSNC_TCCRA = 0; // Clear timer2 register A
	PCICR |= (1 << ULTRSNC_PCIE); // enable pin change interrupt 1
}

int readBatterAmount() {
	isEcho_ = false;
	
	/* Set PC1 to trigger on pin 13, PINC5 */
	ULTRSNC_PCMSK |= (1 << ULTRSNC_PCINT);
	
	/* Pulse trigger pin */
	ULTRSNC_TRGGR_PORT |= (1<<ULTRSNC_TRGGR_PORT_NUM);
	_delay_us(10);
	ULTRSNC_TRGGR_PORT &= ~(1<<ULTRSNC_TRGGR_PORT_NUM);
	
	/* Wait for measurement */
	_delay_ms(10);
	
	/* Calculate amount of pancakes that can be made with current amount of batter 
	float batterVolume = batterLevel_ * CNTNR_W * CNTNR_L;
	int pancakesLeft = batterVolume / DSG_VOL;*/
<<<<<<< HEAD
	DDRB |= 1<<DDB3;
	return (PINB & (1<<PINB3) ? 1 : 0);
	//return batterLevel_; //Skal returnere mellem 80-FF
=======
	
	return 0x40; //Skal returnere 96 for at prog 4 kan k�re flere gange
>>>>>>> master
}
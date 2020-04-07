/*
 * PortPins.h
 *
 * Created: 06/04/2020 12.10.58
 *  Author: au615804
 */ 

/*
Opdeling af timers og pins beskrives i dette segment
Timer 0, compare A bruges af BatterCooler, for at sætte frekvensen af aflæsningen af ADC'en
Timer 1 er sat til 50 af BatterCooler, og bruges både til PWM af køleelementet, samt servomotorer for dispenseren
OCR1A er PWM til køleelementet
OCR1B er PWM til servomotorer
Timer 2 bruges af Ultrasonic aflæseren

PORTB1 bruges af Dej-køleren
PORTB2 bruges af Servo-pwm
PORTB3 bruges af servo-select

PORTC1 bruges af ultrasonic trigger
PINC2 bruges af ultrasonic echo
PINC3 bruges af køler-termometeret
*/

#ifndef PORTPINS_H_
#define PORTPINS_H_





#endif /* PORTPINS_H_ */
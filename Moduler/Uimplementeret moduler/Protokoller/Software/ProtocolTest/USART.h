/*
 * USART.h
 *
 * Created: 27/11/2018 09.00.05
 *  Author: Anton Vigen Smolarz
 */ 


#define F_CPU 16000000
#define X_CLOCK 16000000
#define DDR   DDRE
#define PORT PORTE
#define PINNR 1
#define BAUD  9600

#define NO_us 1000000/(BAUD)


//HW USART
void initUART(unsigned long baudRate, unsigned char dataBit, unsigned char Rx_int, char Parity);
unsigned char charReady();
char readChar();
void sendChar(char sChar);
void sendString(char* sString);
void sendInteger(int sNum);

//SW UART
void SWsendChar(unsigned char sChar);
unsigned char SWreadChar();
void SWsendString(char* sString);
void SWsendInteger(int sNum);


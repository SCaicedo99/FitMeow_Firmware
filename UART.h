#ifndef BAUD
#define BAUD 9600
#endif
#include <avr/io.h>

extern unsigned char MYUBRR;

void UART_Init(void); // It initializes the UART using 9600 baud rate as default
unsigned char UART_ReceiveChar(unsigned int timeout);
void UART_TransmitChar(unsigned char data);
void UART_TransmitStr(char* StringPtr);

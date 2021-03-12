#ifndef BAUD
#define BAUD 9600
#endif
#include <avr/io.h>

#define MYUBRR ((F_CPU/4/BAUD) - 1) / 2

void UART_Init(unsigned char ubrr);
unsigned char UART_ReceiveChar();
void UART_TransmitChar(unsigned char data);
void UART_TransmitStr(char* StringPtr);

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>       // needed for uint8_t
#include "UART.h"

unsigned char MYUBBR = ((F_CPU/4/BAUD) - 1) / 2;

void UART_Init(void){
	// Set Baud rate
	UCSR0A = (1<<U2X0); // Double the UART Trans speed
	
	UBRR0H = (unsigned char)(MYUBBR>>8);
	UBRR0L = (unsigned char)MYUBBR;

	UCSR0C = (3<<UCSZ00); // 8data, 1stop bit, Asynchronous mode, No parity
	
	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0B &= ~(1<<UDRIE0);
}

unsigned char UART_ReceiveChar(unsigned int time_out){
	// Wait for data to be received
	do{
		if (UCSR0A & (1<<RXC0)) return UDR0;
	}while(--time_out);
	return 0;
}

void UART_TransmitChar(unsigned char data){
	// Wait for empty transmit buffer
	while( !(UCSR0A & (1<<UDRE0)) )
	;
	UDR0 = data;
}

void UART_TransmitStr(char* StringPtr){
	while(*StringPtr != 0x00){
		UART_TransmitChar(*StringPtr);    //Using the simple send function we send one char at a time
		StringPtr++;
	}        //We increment the pointer so we can read the next char
}

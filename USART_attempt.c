#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>       // needed for uint8_t

//#define F_CPU 16000000
#define FOSC  16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR ((F_CPU/4/BAUD) - 1) / 2

#define LED _BV(PB0)     // = (1 << PB0)
#define LED_DDR DDRB
#define LED_PORT PORTB

void UART_Init(unsigned char ubrr){
	// Set Baud rate
	UCSR0A = (1<<U2X0); // Double the UART Trans speed
	
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	UCSR0C = (3<<UCSZ00); // 8data, 1stop bit, Asynchronous mode, No parity
	

	// Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
	UCSR0B &= ~(1<<UDRIE0); 
}

unsigned char UART_ReceiveChar(){
	// Wait for data to be received
	while( !(UCSR0A & (1<<RXC0)) )
	;
	
	return UDR0;
}

void UART_TransmitChar(unsigned char data){
	// Wait for empty transmit buffer
	while( !(UCSR0A & (1<<UDRE0)) )
	;
	
	UDR0 = data;
}

void UART_TransmitString(char* StringPtr){
	
	while(*StringPtr != 0x00){
		UART_TransmitChar(*StringPtr);    //Using the simple send function we send one char at a time
		StringPtr++;
	}        //We increment the pointer so we can read the next char
	
}

int main(void)
{
	UART_Init(MYUBRR);
	char string[] = "abcdef \n";
	char recv;
	while(1)
	{
		recv = UART_ReceiveChar();
		if(recv){
			UART_TransmitChar(recv);
		}
	}
}
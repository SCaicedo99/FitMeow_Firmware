#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>       // needed for uint8_t

//#define F_CPU 16000000
#define FOSC  16000000 // Clock Speed
#define BAUD 9600
#define MYUBRR (FOSC/16/BAUD) - 1

#define LED _BV(PB0)     // = (1 << PB0)
#define LED_DDR DDRB
#define LED_PORT PORTB

void USART_Init(unsigned char ubrr){
	// Set Baud rate
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	
	// Enable receiver and transmitter
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0);
	
	// Set frame format: 8 data, 2 stop bit
	//UCSR0C = (1<<USBS0)|(3<<UCSZ00);
	UCSR0C |= (3<<UCSZ00); // 8data, 1stop bit

}

unsigned char USART_ReceiveChar(){
	// Wait for data to be received
	while( !(UCSR0A & (1<<RXC0)) )
	;
	
	return UDR0;
}

void USART_TransmitChar(unsigned char data){
	// Wait for empty transmit buffer
	while( !(UCSR0A & (1<<UDRE0)) )
	;
	
	UDR0 = data;
}

void USART_TransmitString(char* StringPtr){
	
	while(*StringPtr != 0x00){
		USART_TransmitChar(*StringPtr);    //Using the simple send function we send one char at a time
		StringPtr++;
	}        //We increment the pointer so we can read the next char
	
}

int main(void)
{
	USART_Init(MYUBRR);
	char string[] = "abcdef \n";
	char recv;
	while(1)
	{
		recv = USART_ReceiveChar();
		if(recv){
			LED_PORT ^= LED;
			USART_TransmitChar(recv);
		}
		//USART_TransmitChar(0xc1);
		_delay_ms(500);
	}
}
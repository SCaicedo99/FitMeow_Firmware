/*
	This is a demo to show the uart functionality of the 
	ATMega328p UART communication. 

	It is a simple echo server, it replies what it receives
*/

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>       // needed for uint8_t
#include "UART.h"

int main(void)
{
	UART_Init(MYUBRR);
	char recv;
	while(1)
	{
		recv = UART_ReceiveChar();
		if(recv){
			UART_TransmitChar(recv);
		}
	}
}

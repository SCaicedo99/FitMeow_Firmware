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
	char recv[30];
	int i = 0; 
	while(1)
	{
		while(1){
			recv[i] = UART_ReceiveChar(1000);
			if(recv[i] == 0) continue; 
			if(recv[i] == '\n') {
				recv[i+1] = '\0';
				break;
				}
			i++;
		}
		i = 0; 
		// recv = UART_ReceiveChar(1000);
		if(recv){
			UART_TransmitChar(recv);
		}
	}
}

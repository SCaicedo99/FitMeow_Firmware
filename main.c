#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>       // needed for uint8_t
#include <stdio.h>
#include "UART.h"
#include "ADC.h"

#define DEBUG_FLAG 1 // Uncomment/comment to toggle debug prints (sent through uart)

short X_RAW, Y_RAW, Z_RAW;
int X_SCALED, Y_SCALED, Z_SCALED, N_SAMPLES;

uint8_t X_ADC = 0;
uint8_t Y_ADC = 1;
uint8_t Z_ADC = 2;
int RAW_MIN = 0;
int RAW_MAX = 1023;
int SCALED_MIN = -3000;
int SCALED_MAX = 3000;
int SAMPLE_NUMBER = 5;
const int DELAY_MS = 1000;
unsigned int CHECK_CONN_FLAG;

char json_str_format[60] = "{\"X_avg\" : %.2d, \"Y_avg\": %.2d, \"Z_avg\": %.2d}\n";


int check_connection(int timeout){
	char flag = 'H';

	char tmp = UART_ReceiveChar(timeout);
	if(tmp == 0){
		UART_TransmitStr("Check_connection timed out\n");
		return 0;
	} 
	if(tmp == flag){
		UART_TransmitStr("Check_connection received the correct flag\n");
		return 1; 
	} 
	UART_TransmitStr("Did not receive the correct flag\n");
	return -1;
}

int main(void){
	UART_Init();
	_delay_ms(DELAY_MS);

	int n = 1;

	short X_RAW = ADC_avg_sample(X_ADC);
	short Y_RAW = ADC_avg_sample(Y_ADC);
	short Z_RAW = ADC_avg_sample(Z_ADC);

	int SUM_X, SUM_Y, SUM_Z;
	SUM_X = 0;
	SUM_Y = 0;
	SUM_Z = 0;
	
	char buffer[70];
	char bufferN[10];
	
	CHECK_CONN_FLAG = 2*SAMPLE_NUMBER;

	#ifdef DEBUG_FLAG
		UART_TransmitStr("\n\n****** Fitmeow Project ******\n");
		UART_TransmitStr("DELAY_MS = ");
		itoa(DELAY_MS, bufferN, 10);
		UART_TransmitStr(bufferN);
		UART_TransmitStr("     ");
		UART_TransmitStr("SAMPLE_NUMBER = ");
		itoa(SAMPLE_NUMBER, bufferN, 10);
		UART_TransmitStr(bufferN);
		UART_TransmitStr("\n******************************\n");
	#endif
	
	while(1){
		_delay_ms(DELAY_MS); // Take 1 sample every second
		if(!CHECK_CONN_FLAG--){ // Only check connection when check_flag is zero
			check_connection(10000);
			CHECK_CONN_FLAG = 2*SAMPLE_NUMBER; // Reset the check_connection_flag
		}

		#ifdef DEBUG_FLAG
			if(n == 1) UART_TransmitStr("Sample number : \n");
			itoa(n, bufferN, 10);
			UART_TransmitStr(bufferN);
			if(n == SAMPLE_NUMBER) UART_TransmitChar('\n');
			else UART_TransmitChar(' ');
		#endif

		X_RAW = ADC_avg_sample(X_ADC);
		Y_RAW = ADC_avg_sample(Y_ADC);
		Z_RAW = ADC_avg_sample(Z_ADC);

		SUM_X += map(X_RAW, RAW_MIN, RAW_MAX, SCALED_MIN, SCALED_MAX);
		SUM_Y += map(Y_RAW, RAW_MIN, RAW_MAX, SCALED_MIN, SCALED_MAX);
		SUM_Z += map(Z_RAW, RAW_MIN, RAW_MAX, SCALED_MIN, SCALED_MAX);
		
		if(n == SAMPLE_NUMBER){
			sprintf(buffer, json_str_format, SUM_X/SAMPLE_NUMBER, SUM_Y/SAMPLE_NUMBER,
			SUM_Z/n);
			/* Reset the sums and n */
			SUM_X = 0;
			SUM_Y = 0;
			SUM_Z = 0;
			n = 1;

			/* Transmit the data for now */
			UART_TransmitStr(buffer);

			}else{
			n++;
		}

	}

	return 0;
}

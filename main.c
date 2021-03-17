#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>       // needed for uint8_t
#include <stdio.h>
#include "UART.h"
#include "ADC.h"

uint8_t X_ADC, Y_ADC, Z_ADC;
short X_RAW, Y_RAW, Z_RAW;
int X_SCALED, Y_SCALED, Z_SCALED, N_SAMPLES;

int RAW_MIN = 0;
int RAW_MAX = 1023;
int SCALED_MIN = -3000;
int SCALED_MAX = 3000;
int SAMPLE_NUMBER = 5;
const int DELAY_MS = 1000;

short test[100];

int check_connection(int timeout){
	char flag = 'H';
	// char buffer[10];
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
	X_ADC = 0;
	Y_ADC = 1;
	Z_ADC = 2;

	int n = 1;
	int check_connection_flag = 2*SAMPLE_NUMBER; 

	short X_RAW = ADC_avg_sample(X_ADC);
	short Y_RAW = ADC_avg_sample(Y_ADC);
	short Z_RAW = ADC_avg_sample(Z_ADC);

	int SUM_X, SUM_Y, SUM_Z;
	SUM_X = 0;
	SUM_Y = 0;
	SUM_Z = 0;

	char str_fmt[70] = "X_avg = %.2d, Y_avg %.2d, Z_avg %d \n";
	
	char buffer[70];
	char bufferN[5];
	UART_Init();

	while(1){
		_delay_ms(DELAY_MS); // Take 1 sample every second
		if(!check_connection_flag--){ // Only check when check_flag is zero
			check_connection(10000);
			check_connection_flag = 2*SAMPLE_NUMBER; // Reset the check_connection_flag
		} 
		X_RAW = ADC_avg_sample(X_ADC);
		Y_RAW = ADC_avg_sample(Y_ADC);
		Z_RAW = ADC_avg_sample(Z_ADC);

		SUM_X += map(X_RAW, RAW_MIN, RAW_MAX, SCALED_MIN, SCALED_MAX);
		SUM_Y += map(Y_RAW, RAW_MIN, RAW_MAX, SCALED_MIN, SCALED_MAX);
		SUM_Z += map(Z_RAW, RAW_MIN, RAW_MAX, SCALED_MIN, SCALED_MAX);
		
		itoa(SUM_Z, bufferN, 10);
		UART_TransmitStr(bufferN);
		UART_TransmitChar('\n');
		if(n == SAMPLE_NUMBER){
			sprintf(buffer, str_fmt, SUM_X/SAMPLE_NUMBER, SUM_Y/SAMPLE_NUMBER,
			SUM_Z/n);
			itoa(SAMPLE_NUMBER, bufferN, 10);
			SUM_X = 0;
			SUM_Y = 0;
			SUM_Z = 0;
			UART_TransmitStr(buffer);
			UART_TransmitStr(bufferN);
			UART_TransmitChar('\n');
			n = 1;
			}else{
			n++;
		}
		UART_TransmitStr("=========\n");
		// check_connection(10000);
	}

	return 0;
}

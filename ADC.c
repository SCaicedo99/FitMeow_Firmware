#include <avr/io.h>
#include <stdint.h>       // needed for uint8_t
#include "ADC.h"

short ADC_single_read(uint8_t adctouse)
{
	short ADCval;

	ADMUX = adctouse;         // use #1 ADC
	ADMUX |= (1 << REFS0);    // use AVcc as the reference
	ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution
	
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescale for 8Mhz
	ADCSRA |= (1 << ADEN);    // Enable the ADC

	ADCSRA |= (1 << ADSC);    // Start the ADC conversion

	while(ADCSRA & (1 << ADSC));      // This line waits for the ADC to finish


	ADCval = ADCL;
	ADCval = (ADCH << 8) + ADCval;    // ADCH is read so ADC can be updated again

	return ADCval;
}

short ADC_avg_sample(uint8_t adctouse){
    short sum = 0;
    int i;
    for(i = 0; i < N_AVG_SAMPLES; i++)
        sum += ADC_single_read(adctouse);
    
    return (short) (sum/N_AVG_SAMPLES);
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
	// Re maps a number from one range to another
	return ((x - in_min) * 7.82) + out_min; // map function from arduino code
}
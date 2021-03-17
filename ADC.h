#include <avr/io.h>
#include <stdint.h>       // needed for uint8_t

#ifndef F_CPU
#define F_CPU   1000000UL
#endif

#define N_AVG_SAMPLES 10  // Number of samples taken in the ADC_avg_sample

/*  Reads from the ADC passed in as an unsigned 8 bit number, in the case of the 
    ATMega328p there are 6 different ADCs that can be used (0-5), it returns a value
    from 0 - 1023 (10 bit resolution)
*/
short ADC_single_read(uint8_t adctouse);

/*  This function takes in an int x within a range [in_min, in_max] and outputs an 
    equivalent number in the range [out_min, out_max]
*/
int map(int x, int in_min, int in_max, int out_min, int out_max);

/*  Returns the avg of N_AVG_SAMPLES from the ADC passed in as a parameter, this is done
    in order to smooth out any noise
*/
short ADC_avg_sample(uint8_t adctouse);

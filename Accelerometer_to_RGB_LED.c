#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>       // needed for uint8_t
#define F_CPU   1000000UL

// Got this from https://sites.google.com/site/qeewiki/books/avr-guide/pwm-on-the-atmega328
int ADC_single_read(uint8_t adctouse)
{
	int ADCval;

	ADMUX = adctouse;         // use #1 ADC
	ADMUX |= (1 << REFS0);    // use AVcc as the reference
	ADMUX &= ~(1 << ADLAR);   // clear for 10 bit resolution
	
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);    // 128 prescale for 8Mhz
	ADCSRA |= (1 << ADEN);    // Enable the ADC

	ADCSRA |= (1 << ADSC);    // Start the ADC conversion

	while(ADCSRA & (1 << ADSC));      // Thanks T, this line waits for the ADC to finish


	ADCval = ADCL;
	ADCval = (ADCH << 8) + ADCval;    // ADCH is read so ADC can be updated again

	return ADCval;
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
	// Re maps a number from one range to another
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min; // map function from arduino code
}

int inRange(float low, float high, float x)
{
	return ((x-high)*(x-low) <= 0);
}

int main(void)
{
	DDRD |= (1 << DDD6);
	// PD6 is now an output

	OCR0A = 128;
	// set PWM for 50% duty cycle


	TCCR0A |= (1 << COM0A1);
	// set none-inverting mode

	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	// set fast PWM Mode

	TCCR0B |= (1 << CS01);
	// set prescaler to 8 and starts PWM

	int raw;
	float axixScaled;
	
	while(1)
	{
		 // This will read from ADC3 and map it to gravitational acceleration (G)
		raw = ADC_single_read(1);
		axixScaled = map(raw, 0, 1023, 0, 255)/1.0;
		OCR0A = (int) axixScaled;
		/*if(inRange(-2000.0, 2000.0, axixScaled)) OCR0A = 255; // Turn LED on if 1G is on the axis being sampled
		else {
			OCR0A = 0;
		}*/
		
	}
}
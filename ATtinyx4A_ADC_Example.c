/*
 * ATtinyx4A_ADC_Example.c
 *
 * Created: 02/08/2016
 * Author: Will Sakran
 * Metre Ideas and Design
 *
 * This program reads an analog input on ADC0 (pin PA0). An external interrupt
 * (INT0 on pin PB2) is also provided which can be used to trigger the chip to
 * store the current analog input value to a reference variable. Current samples
 * can then be compared to this reference value.
 *
 * Developed for use with Atmel ATtiny24A / ATtiny44A / ATtiny84A
 * For ATtiny24A/44A/84A Prototyping boards go to
 * www.metreideas.com
 *
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

/* Set the correct clock frequency based on fuse settings or external clock/crystal
 * has to be done before including delay.h */
#define F_CPU	1000000UL
#include <util/delay.h>

volatile uint16_t	reference=0;
volatile uint16_t	sample=0;

void board_init(void);
uint16_t get_ADC_sample(void);

/**
 * \brief Main routine
 */
int main(void)
{
	uint16_t	deviation;
	
	board_init();
		
	sei();							// Enable interrupts
	
	// Set up the external interrupt	
	MCUCR |= (1 << ISC01) | (1 << ISC00);	// set INT0 to trigger on a rising edge
	GIMSK |= (1 << INT0);					// enable the external interrupt
	GIFR |= (1 << INTF0);					// clear the external interrupt flag
	
	while (1) {
		
		// read four samples and then average them
		sample = get_ADC_sample();
		sample += get_ADC_sample();
		sample += get_ADC_sample();
		sample += get_ADC_sample();
		sample = sample >> 2;			// divide by 4 to average the samples
		
		// The 'deviation' variable isn't being used for anything in this example,
		// it's simply storing the difference between the reference value and
		// the current sample value.
		if (sample > reference)
			deviation = sample - reference;
		else
			deviation = reference - sample;

	} // end while
} // end main


uint16_t get_ADC_sample(void)
{
	// start a conversion by setting the ADSC bit in ADCSRA
	ADCSRA |= (1 << ADSC);
	
	// wait for the conversion to complete
	while ((ADCSRA & 0x40) == 0x40) {
		asm("nop");
	}
	
	// return the ADC value
	return(ADC);
}


/* External interrupt vector */
ISR(EXT_INT0_vect)
{
	uint8_t	i;
	uint16_t value;
	
	// get the reference value by averaging several samples
	reference = 0;
	value = get_ADC_sample();	// get one sample and discard it
	
	for(i=0 ; i < 4 ; i++) {
		value = get_ADC_sample();
		reference += value;
	}
	
	reference = (reference >> 2);
}


void board_init(void)
{
	// ADMUX defaults to Vcc reference with ADC0 as input

	// Enable ADC with prescale by 8
	ADCSRA = (1<<ADEN) | (1<<ADPS1) | (1<<ADPS0);

	// Disable digital input on ADC0 pin. Not strictly required
	// but reduces power consumption a bit.
	DIDR0 = 0x01;

	PORTA = 0xFE;	// enable pull-ups on PA7 - PA1
	PORTB = 0x0B;	// enable pull-ups on PB3, PB1, PB0
}
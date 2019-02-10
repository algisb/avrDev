#include <avr/io.h> 
#include <util/delay.h>
#include <avr/interrupt.h>

#define BLINK_DELAY_MS 1000
#define BLINK_DELAY_MS_2 100


ISR(INT0_vect)
{
	unsigned int i;
	//_delay_ms(500); // Software debouncing control
 
	/* This for loop blink LEDs on Dataport 5 times*/
	for(i = 0; i<20; i++)
	{
		PORTB = 0b010000;
		_delay_ms(BLINK_DELAY_MS_2);	// Wait 5 seconds
		PORTB = 0b000000;
		_delay_ms(BLINK_DELAY_MS_2);	// Wait 5 seconds
	}	
}	




 
int main (void)
{
	DDRD = 1<<PD2;		// Set PD2 as input (Using for interupt INT0)
	PORTD = 1<<PD2;		// Enable PD2 pull-up resistor
 
	DDRB = 0xFF;		// Configure Dataport as output
	PORTB = 0x00;	// Initialise Dataport to 1
 
	EIMSK = 1<<INT0;					// Enable INT0
	MCUCR = 1<<ISC01 | 1<<ISC00;	// Trigger INT0 on rising edge
 
	sei();				//Enable Global Interrupt


	/* set pin 5 of PORTB for output*/
	//DDRB |= _BV(DDB5);
 	//DDRB = 0b010000;
	while(1)
	{
		/* set pin 5 high to turn led on */
		//PORTB |= _BV(PORTB5);
		
		PORTB = 0b010000;
		_delay_ms(BLINK_DELAY_MS);
		
		/* set pin 5 low to turn led off */
		//PORTB &= ~_BV(PORTB5);
		PORTB = 0b000000;
		_delay_ms(BLINK_DELAY_MS);
	 }
}


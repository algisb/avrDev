#include <avr/io.h> 
#include <util/delay.h>

// define some macros
//#define BAUD 9600                                   // define baud
//#define BAUDRATE ((F_CPU)/(BAUD*16UL)-1)            // set baud rate value for UBRR


#define FOSC 1843200 // Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#define BLINK_DELAY_MS 200
#include <util/setbaud.h>

void USART_Init(unsigned int ubrr)
{
	/*
	Set baud rate 
	*/
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;

	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* 
	Set frame format: 8data, 2stop bit 
	*/
	UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

unsigned char USART_Receive(void)
{
	/* 
	Wait for data to be received
	*/
	while(!(UCSR0A & (1<<RXC0)));
	/* 
	Get and return received data from buffer
	*/
	return UDR0;
}

int main (void)
{
	/* set pin 5 of PORTB for output*/
	//DDRB |= _BV(DDB5);
 	DDRB = 0b110000;
	int gotChar = 0;
	USART_Init(MYUBRR);

	while(1)
	{
		if(gotChar == 0)
		{
			USART_Receive();
			gotChar = 1;
		}

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


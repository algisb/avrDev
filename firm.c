#include <avr/io.h> 
#include <util/delay.h>
#include <stdio.h> 

#define BLINK_DELAY_MS 100

#define LCD_REG_SELECT 0
#define LCD_READ_WRITE 1
#define LCD_ENABLE 2

#define DATA_PORT PORTD
#define COMMAND_PORT PORTB

#define F_CPU 16000000UL
#define BAUD 9600

#include <util/setbaud.h>
void uart_init(void)
{
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

//#if USE_2X
//	UCSR0A |= 1 << U2X0;
//#else
	UCSR0A &= ~(1 << U2X0);
//#endif
	
	UCSR0C = 1 << UCSZ01 | 1 << UCSZ00; /* 8-bit data */
	UCSR0B = 1 << RXEN0 | 1 << TXEN0;   /* Enable RX and TX */
}

void uart_putchar(char c)
{
	UDR0 = c;
	loop_until_bit_is_set(UCSR0A, TXC0); /* Wait until transmission ready. */
}

char uart_getchar(void) 
{
	loop_until_bit_is_set(UCSR0A, RXC0); /* Wait until data exists. */
	return UDR0;
}


void LCD_command( unsigned char _cmnd )
{
	//set port direction to output
	DDRD |= 0xF0;
	DDRB |= 0b000111;

	
	COMMAND_PORT &= ~(1 << LCD_READ_WRITE);	
	DATA_PORT = (DATA_PORT & 0x0F) | (_cmnd & 0xF0);/* Sending upper nibble */
	
	COMMAND_PORT &= ~(1 << LCD_REG_SELECT);		/* RS=0, command reg. */
	COMMAND_PORT |= (1 << LCD_ENABLE);		/* Enable pulse */
	_delay_ms(1);
	COMMAND_PORT &= ~(1 << LCD_ENABLE);
	_delay_ms(1);
	DATA_PORT = (DATA_PORT & 0x0F) | (_cmnd << 4);/* Sending lower nibble */
	COMMAND_PORT |= (1 << LCD_ENABLE);
	_delay_ms(1);
	COMMAND_PORT &= ~(1 << LCD_ENABLE);
	_delay_ms(1);
}


void LCD_char( unsigned char _char )
{
	//set port direction to output
	DDRD |= 0xF0;
	DDRB |= 0b000111;

	COMMAND_PORT &= ~(1 << LCD_READ_WRITE);	
	DATA_PORT = (DATA_PORT & 0x0F) | (_char & 0xF0);/* Sending upper nibble */
	
	COMMAND_PORT |= (1 << LCD_REG_SELECT);		/* RS=1, data reg. */
	COMMAND_PORT |= (1 << LCD_ENABLE);		/* Enable pulse */
	_delay_ms(1);
	COMMAND_PORT &= ~(1 << LCD_ENABLE);
	_delay_ms(1);
	DATA_PORT = (DATA_PORT & 0x0F) | (_char << 4);/* Sending lower nibble */
	COMMAND_PORT |= (1 << LCD_ENABLE);
	_delay_ms(1);
	COMMAND_PORT &= ~(1 << LCD_ENABLE);
	_delay_ms(2);
}


void LCD_clear()
{
	LCD_command (0x01);		/* Clear display */
	_delay_ms(2);
	LCD_command (0x80);		/* Cursor at home position */
}

void LCD_string(char * _str)
{
	int i;
	int onSecondLine = 0;
	for(i=0;_str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
		if(i > 31)
			break;

	
		if(onSecondLine == 0)
		{
			if(_str[i] == '\n' || i > 15)
			{
			
				LCD_command(0xC0);
				onSecondLine = 1;
			}
		}
		LCD_char (_str[i]);
	}	
}

void LCD_reset()
{

	DDRD |= 0xF0;
	DDRB |= 0b000111;
	//send 0x03 to the LCD 3 times to make sure its in its inital 8 bit state	
	int i;
	for(i = 0; i < 3; i++)
	{
		COMMAND_PORT &= ~(1 << LCD_READ_WRITE);	
		DATA_PORT = 0x03;
		COMMAND_PORT &= ~(1 << LCD_REG_SELECT);		/* RS=0, command reg. */
		COMMAND_PORT |= (1 << LCD_ENABLE);		/* Enable pulse */
		_delay_ms(1);
		COMMAND_PORT &= ~(1 << LCD_ENABLE);
		_delay_ms(1);
	}
}

void LCD_init()
{
		
	LCD_reset();
	_delay_ms(20);			/* LCD Power ON delay always >15ms */
	
	LCD_command(0x02);		/* send for 4 bit initialization of LCD  */
	LCD_command(0x28);              /* 2 line, 5*7 matrix in 4-bit mode */
	LCD_command(0x0c);              /* Display on cursor off*/
	LCD_command(0x06);              /* Increment cursor (shift cursor to right)*/
	LCD_command(0x01);              /* Clear display screen*/
	_delay_ms(2);
	

}

int main (void)
{
	LCD_init();	
	uart_init();	
	LCD_string("hello world!");
	while(1)
	{
		char line[32];
		int i = 0;
		while(1)
		{
			line[i] = uart_getchar();
			if (line[i] == '\n') break;
			i++;
		}
		line[i] = 0;
		
		
		//	buff[i] = data;
		//	i++;
		//}
		//char buff[32];
		
		
		//sprintf(buff, "Data: %c", data);

		LCD_clear();
		LCD_string(line);
		
		//uart_putchar('k');	
		//_delay_ms(3000);
		//_delay_ms(BLINK_DELAY_MS);
		//_delay_ms(BLINK_DELAY_MS);
	}
}


#include <avr/io.h> 
#include <util/delay.h>
#include <stdio.h> 

#define BLINK_DELAY_MS 100

#define LCD_REG_SELECT 0
#define LCD_READ_WRITE 1
#define LCD_ENABLE 2

#define DATA_PORT PORTD
#define COMMAND_PORT PORTB




void LCD_command( unsigned char _cmnd )
{
	//DDRD = 0xFF;
	//DDRB = 0b001111;

	//DATA_PORT = 0b00000000;
	//COMMAND_PORT = 0b000000;
	

	COMMAND_PORT &= ~(1 << LCD_READ_WRITE);	
	DATA_PORT = _cmnd;/* Sending upper nibble */
	
	COMMAND_PORT &= ~(1 << LCD_REG_SELECT);		/* RS=0, command reg. */
	COMMAND_PORT |= (1 << LCD_ENABLE);		/* Enable pulse */
	_delay_ms(1);
	COMMAND_PORT &= ~(1 << LCD_ENABLE);
	_delay_ms(1);
	DATA_PORT = (_cmnd << 4);/* Sending lower nibble */
	COMMAND_PORT |= (1 << LCD_ENABLE);
	_delay_ms(1);
	COMMAND_PORT &= ~(1 << LCD_ENABLE);
	_delay_ms(1);
}


void LCD_char( unsigned char _char )
{
	//DDRD = 0xFF;
	//DDRB = 0b001111;

	//DATA_PORT = 0b00000000;
	//COMMAND_PORT = 0b000000;
	

	COMMAND_PORT &= ~(1 << LCD_READ_WRITE);	
	DATA_PORT = _char;/* Sending upper nibble */
	
	COMMAND_PORT |= (1 << LCD_REG_SELECT);		/* RS=1, data reg. */
	COMMAND_PORT |= (1 << LCD_ENABLE);		/* Enable pulse */
	_delay_ms(1);
	COMMAND_PORT &= ~(1 << LCD_ENABLE);
	_delay_ms(1);
	DATA_PORT = (_char << 4);/* Sending lower nibble */
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
		//LCD_command(0xC0);
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
	DDRD = 0xFF;
	DDRB = 0b001111;
	
	DATA_PORT = 0b00000000;
	COMMAND_PORT = 0b000000;

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
	LCD_string("hello world!");
	while(1)
	{
		int i;
		for(i = 0; i < 4; i++)
		{
			LCD_clear();
			char buff[4];
			sprintf(buff, "%d", i+1);
			LCD_string(buff);
			_delay_ms(1000);
			
		}
		
		LCD_clear();
		LCD_string("Hello world!");
		_delay_ms(1000);
		//_delay_ms(BLINK_DELAY_MS);
		//_delay_ms(BLINK_DELAY_MS);
	}
}


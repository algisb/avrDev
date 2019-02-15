#include <avr/io.h> 
#include <util/delay.h>
 
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
	int i;
	for(i=0;_str[i]!=0;i++)		/* Send each char of string till the NULL */
	{
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
		//LCD_char('k');
	//LCD_command(0x0c);

		//checkIfBusy();
		/* set pin 5 high to turn led on */
		//PORTB |= _BV(PORTB5);
		
		//PORTB |= (1 << 3);
		//PORTD = 0b00000000;
		//LCD_Command(0x06);
		_delay_ms(BLINK_DELAY_MS);
		//LCD_Clear();
		//LCD_Command(0x01);
		/* set pin 5 low to turn led off */
		LCD_command(0x01);
		LCD_string("yep! ");	
		//PORTB |= (1 << 3);
		//PORTB &= ~_BV(PORTB5);
		//PORTB ^= 0b001000;
		//PORTB = 0b000000;
		//PORTD = 0b00000000;
		//LCD_Command(0x06);
		_delay_ms(BLINK_DELAY_MS);
	 }
}


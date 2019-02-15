#avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -I './lcdLib/' -c -o firm.o firm.c

#avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o lcd.o lcdLib/lcd.c

#avr-gcc -mmcu=atmega328p firm.o lcd.o -o firm

avr-gcc -Os -DF_CPU=16000000UL -mmcu=atmega328p -c -o firm.o firm.c

avr-gcc -mmcu=atmega328p firm.o -o firm
avr-objcopy -O ihex -R .eeprom firm firm.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/ttyACM0 -b 115200 -U flash:w:firm.hex

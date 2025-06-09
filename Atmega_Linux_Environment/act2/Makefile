MCU=atmega32
F_CPU=1000000UL
CC=avr-gcc
CFLAGS=-mmcu=$(MCU) -DF_CPU=$(F_CPU) -Os
TARGET=main_input

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET).elf $(TARGET).c
	avr-objcopy -O ihex -R .eeprom $(TARGET).elf $(TARGET).hex

clean:
	rm -f *.o *.elf *.hex

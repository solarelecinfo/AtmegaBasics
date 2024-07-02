# Nom du fichier cible (sans extension)
TARGET = main

# Microcontrôleur cible
MCU = atmega32

# Fréquence du microcontrôleur en Hz default avr internal clock =1MHZ
F_CPU = 1000000UL

# Options de compilation
CFLAGS = -g -Wall -Os -std=c99 -mmcu=$(MCU) -DF_CPU=$(F_CPU)

# Commandes pour la compilation et la programmation
CC = avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude

# Liste des fichiers sources
SRC = $(TARGET).c

all: $(TARGET).hex

# Compilation du code source en fichier hex
$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

# Compilation du code source en fichier elf
$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $^ -o $@

# Programmation du microcontrôleur
program: $(TARGET).hex
	$(AVRDUDE) -p $(MCU) -c usbasp -U flash:w:$<

# Nettoyage des fichiers générés
clean:
	rm -f $(TARGET).elf $(TARGET).hex

.PHONY: all program clean
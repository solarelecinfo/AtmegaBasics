#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


/**
Le programme impl�mente le bouton PC7, permettant de g�n�rer un nombre al�atoire de transitions LED ON/OFF.
**/
void setup();

int main(void){
	setup();
	while(1) {
		if(!(PINC & (1 << PINC7))) { //V�rifier si le bouton a �t� appuy� ,c.�.d PINC7==0
			PORTB ^= (1 << PINB1) | (1 <<PINB0);   // 0b000001100 XOR PORTB pour clignoter les sorties
			_delay_ms(600);
		}
	
	}
	return 0;
}


/**
Description : initialisation des registres GPIO en sortie et en entr�e
**/
void setup(){
	// Initialisation de l'entr�e
	DDRC &= ~(1 << PINC7); // Configurer PINC7 comme entr�e
	PORTC = 0b10000000; // Configurer l'entr�e PINC7 en mode PULL-UP (5V sont mis en sortie lorsqu'elle est inactive)
		
	// Initialisation de la sortie
	DDRB |= (1 << PINB1) | (1 << PINB0);// Configurer PINB2 et PINB3 comme sorties
	PORTB &= 0b00000000; // Forcer l'�tat initial du port � z�ro
	

}
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
		uint8_t randomNumber = (rand() % 9) + 2; // G�n�ration d'un nombre al�atoire entre 2 et 10
	
		if(!(PINC & (1 << PINC7))){ //V�rifier si le bouton a �t� appuy� ,c.�.d PINC7==0
			for(int i=0; i<randomNumber; i++){
				PORTB ^= (3 << PINB2);   // 0b00000110 XOR PORTB pour clignoter les sorties 
				_delay_ms(500);
			}
		} else {
			PORTB &= ~((1 << PINB2) | (1 << PINB3));
		}
	}
return 0;
}


/**
Description : initialisation des registres GPIO en sortie et en entr�e
**/
void setup(){
	// Initialisation de la sortie
	srand(1);//Initialiser la fonction de generation de nombre aleatoire
	DDRB |= (1 << PINB2) | (1 << PINB3);// Configurer PINB2 et PINB3 comme sorties 
	PORTB &= 0b00000000; // Forcer l'�tat initial du port � z�ro
	
	 // Initialisation de l'entr�e
	DDRC &= ~(1 << PINC7); // Configurer PINC7 comme entr�e
	PORTC = 0b10000000; // Configurer l'entr�e PINC7 en mode PULL-UP (5V sont mis en sortie lorsqu'elle est inactive)
}
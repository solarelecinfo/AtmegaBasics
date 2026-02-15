#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


/**
Le programme implémente le bouton PC7, permettant de générer un nombre aléatoire de transitions LED ON/OFF.
**/
void setup();

int main(void){
	setup();
	while(1) {
		uint8_t randomNumber = (rand() % 9) + 2; // Génération d'un nombre aléatoire entre 2 et 10
	
		if(!(PINC & (1 << PINC7))){ //Vérifier si le bouton a été appuyé ,c.à.d PINC7==0
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
Description : initialisation des registres GPIO en sortie et en entrée
**/
void setup(){
	// Initialisation de la sortie
	srand(1);//Initialiser la fonction de generation de nombre aleatoire
	DDRB |= (1 << PINB2) | (1 << PINB3);// Configurer PINB2 et PINB3 comme sorties 
	PORTB &= 0b00000000; // Forcer l'état initial du port à zéro
	
	 // Initialisation de l'entrée
	DDRC &= ~(1 << PINC7); // Configurer PINC7 comme entrée
	PORTC = 0b10000000; // Configurer l'entrée PINC7 en mode PULL-UP (5V sont mis en sortie lorsqu'elle est inactive)
}
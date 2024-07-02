#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 1000000UL

/**Button PC7 sert à detecter une voiture qui entre au parking
	la lampe s'alumme(PORTB0) à chaque déetction,
	la porte doit fermer si MAX atteint est allume LED2(PORTB1)
**/
unsigned int compteur=0;

void setup();

int main(void){
	setup();
	while(1){
		if( !(PINC & (1 << PINC7)) ){ //Capteur détecte LOW=0V
			PORTB |= (1<<PINB0);  /*0b0000000 Clignoter PINB2 */ 
			_delay_ms(1000);
			PORTB &= ~(1<<PINB0);
			compteur++;
		}
		if( compteur >= 7 ){
			PORTB |= (1<<PINB1);
		}
	}
	return 0;
}

void setup(){
	//ENTREE CAPTEUR
	DDRC = 0b00000000 ; /* 0x00 //PORTC en INPUT*/
	PORTC=0b10000000; /* Configurer PINC7 en mode PULL-UP(Forcer lire HIGH-5V si l'entrée est inactive)*/
	
	//SORTIE MOTEUR
	DDRB |= 0b00000011;/*0x03 PIN0,PIN1 du port PORTB en OUTPUT*/ 
	PORTB =0b00000000; /*Initialiser état du sortie à LOW 0*/
	
	_delay_ms(200);
}
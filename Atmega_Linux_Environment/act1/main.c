#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

/** Faire clignoter deux LEDS*/
int main(void){
	DDRB |= ( 1 << PINB2)|( 1 << PINB3);//0b00001100 configuration pins PORTB en OUTPUT*/
	PORTB = 0b00000100; //Définir état initial PORTB
	
	while(1){
	PORTB ^= (1 << PINB2)|(1 << PINB3);  /*XOR basculer les sorties des PINS*/
	_delay_ms(1000);
	}

	return 0;
}


#define F_CPU 16000000UL //Frequence par default pour arduino UNO
#include <avr/io.h>
#include <util/delay.h>

int main(void){
	DDRB |= (1<<PINB2)|(1<<PINB3);/*0b00001100 configuration pins en OUTPUT*/ 
	PORTB=0b00000100; //Définir état initial du PINB2
	
	while(1){
		PORTB ^= (1<<PINB2)|(1<<PINB3);  /*XOR en sortie du pin pour faire clignoter la tension */ 
		_delay_ms(1000);
	}

return 0;
}
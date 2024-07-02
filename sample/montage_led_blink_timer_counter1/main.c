#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 1000000UL
volatile unsigned int increment = 0;
unsigned char VALIDATOR = 0x00;

/**Button example
button PC7 will triger a random nunmber to tick every x seconds"**/
void setup();

int main(void){
	increment =0;
	setup();
	while(1){
		
	}
return 0;
}



void setup(){
	//PORTC CONFIGURATION SPECIAL PINB3 en sortie OC0(signal du compteur) 
	DDRA |= 0x00;
	PORTA = 0x00; /*initial state of pins as output*/
	asm( "nop ");

	//PORTC CONFIGURATION SPECIAL PINB3 en sortie OC0(signal du compteur) 
	DDRC =0xFF; /* 0b00000001//PINC0 as OUTPUT*/
	PORTC = 0b00000000; /*initial state of pins as output*/
	asm( "nop ");

	//PORTB CONFIGURATION SPECIAL PINB3 en sortie OC0(signal du compteur) 
	DDRB |= (1<<PINB3); /* 0b00001000//PINB3 as OUTPUT*/
	PORTB =0b00000000;/*initial state of pins as output*/
	_delay_ms(200);


	//TIMER CONFIGURATION
	TCCR0 |= (1<<WGM01); //configurer le mode du timer en CTC valeur max du compteur customisable 
	TCCR0 |= (1<<COM00); //configurer le pin OC0 de seuil de comptage en mode TOGGLE à chaque MAX_TOP
	TCCR0 |= (1<<CS02)|(1<<CS00); //configurer prescalaire du 8 bit compteur0 en 1MHZ/1024
	
	OCR0 = 244;//MAX_TOP customisable counter value
	
	//INTERRUPT
	sei();
	TIMSK |= (1<<OCIE0);
	TIFR |= (1<<OCF0);
}


ISR(TIMER0_COMP_vect){
	increment++;
	if(increment >4){
		PORTC ^= (1<<PORTC0);
		increment = 0;
	}
}

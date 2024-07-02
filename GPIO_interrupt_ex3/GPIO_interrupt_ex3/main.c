#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Déclaration prototype de fonctions
void setupGpio(void);
void setUpGpioInterrupt(void);

/** Faire clignoter deux LEDS*/
int main(void){	
	setupGpio();
	setUpGpioInterrupt();
	while(1){
		asm( "nop" );
	}
return 0;
}

/** Configuration PORTC en sortie **/
void setupGpio(){
	DDRB = 0b00001000; //PINB3 PRTB en OUTPUT
	PORTB = 0b00000000; 
}

/**	Configuration Interruption GPIO front montant/descendent **/
void setUpGpioInterrupt(){
	DDRD = 0b00000000;//PORTD en INPUT*/ 
	PORTD |= (1 << PIND2); //0bxxxxx1xx Configurer PIN INT0 en PULLUP INPUT 5V
	//Interruption GPIO
	sei();
	GICR |= (1 << INT0); //Activer interruption sur PIN INTO 
	MCUCR |= (1 << ISC01) | (1 << ISC00); //Déclencher interruption au front montant 0->5V
}

/**Routine d'interruption PIN montant/descendent et raz de flag d'interruption automatique**/
ISR(INT0_vect){
		PORTB ^= (1<<PINB3);
}
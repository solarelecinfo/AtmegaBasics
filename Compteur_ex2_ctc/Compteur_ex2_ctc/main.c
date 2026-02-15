#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void setupGPIO(void);
void setupTimer(void);

unsigned int increment=0;

/**Timer en mode CTC avec interruption, bascule LED1=1s et bascule LED2=3s **/

int main(void){
	setupGPIO();
	setupTimer();
	while(1){
		asm( "nop ");
	}
return 0;
}


/** Configuration PORTC en sortie **/
void setupGPIO(){
	DDRC |= 0x01;
	PORTC = 0x00;
}

/**	Configuration TIMER1 (16Bit)=65536 **/
void setupTimer(){
	DDRD |= (1 << PIND5); //PIN OC1A en OUTPUT
	TCCR1A |= (1 << COM1A0); //Sortie PIN OC1A en mode Bascule ON/OFF .   
	TCCR1B |= (1 << WGM12); //Fixer Mode CTC en interne et le signal de sortie.
	TCCR1B |= (1 << CS11) | ( 1<< CS10); //Config prescalaire en 1MHZ/64 = 15625Hz
	OCR1A = 15624; //Seuil comptage MAX_TOP=1s=(MAX-1)
	
	//config interruption
	sei();//Autoriser les interruptions
	TIMSK |= (1 << OCIE1A); //Activer l'interruption CTC  
}


/** Sous routine d'interruption CTC ,actif lorsque le comptage atteint le MAX , raz de flag automatique*/ 
ISR(TIMER1_COMPA_vect){
	increment = (increment +1) % 3;
	if(increment == 0){
		PORTC ^= (1<<PORTC0);
		}
}


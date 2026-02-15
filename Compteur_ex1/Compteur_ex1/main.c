#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>


void setupGPIO(void);
void setupTimer(void);

/**Timer en mode CTC sans interruption, bascule LED1=1s et bascule LED2=2s **/

int main(void){
	setupGPIO();
	setupTimer();
	while(1){
		if ( !(TCNT1 % 7810) ){
			PORTB ^= 0x01; //LED ON
		}else{
			//PORTB = 0x00; //LED OFF
		}
	}
return 0;
}


/** Configuration PORTC en sortie **/
void setupGPIO(){
	DDRB |= 0x01;
	PORTB = 0x00;
	

}

/**	Configuration TIMER1 (16Bit)=65536 **/
void setupTimer(){
	TCCR1A |= (1 << COM1A0); //Sortie PIN OC1A en mode Bascule ON/OFF .   
	TCCR1B |= (1 << WGM12); //Fixer Mode CTC en interne et le signal de sortie.
	TCCR1B |= (1 << CS11) | ( 1<< CS10); //Config prescalaire en 1MHZ/64 = 15625Hz
	OCR1A = 31250; //Seuil comptage MAX_TOP=2s=2x(MAX-1)
}


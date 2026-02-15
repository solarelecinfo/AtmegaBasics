#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

void setupGPIO(void);
void setupTimer(void);

/**Example of a 16 Bit timer in CTC mode with no interruption, Blink LED1 based on OC1A frequency = 0,5Hz =2 seconds (1 second ON, 1 second OFF) **/

int main(void){
	setupGPIO();
	setupTimer();

	while(1){}
	return 0;
}


/** Configuration PORTB as OUTPUT **/
void setupGPIO(){
	DDRD |= (1 << PD5);//Enable OC1A Physical pin as output 
}

/**	Configuration TIMER1 (16Bit) in Nomal mode ,  reset done every OCR1A counts **/
void setupTimer(){
	TCCR1A |= (1 << COM1A0); //Activate PIN OC1A (toggle ON/OFF).
	TCCR1B |= (1 << WGM12); //Set Timer1 in CTC Mode with top as OCR1A value.
	TCCR1B |= (1 << CS11) | (1 << CS10) ; //Config frequency prescaler of timer to 1MHZ/64 = 15625Hz
    OCR1A = 15625;// Set timer TOP reset threshold to 15625 counts)
}


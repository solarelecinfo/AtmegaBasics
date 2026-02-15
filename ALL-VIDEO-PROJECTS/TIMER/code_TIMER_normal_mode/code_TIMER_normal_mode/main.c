#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

void setupGPIO(void);
void setupTimer(void);

/**Example of a 16 Bit timer in Normal mode, Blink LED1 aproximately every 2 second **/

int main(void){
	setupGPIO();
	setupTimer();

	while(1){
		//Read current value of timer and blink approximately every 2 seconds (half timer period = (2^16)/2 =65536/2 = 32768 counts, equivalent to 2,09 seconds)
		if ( TCNT1 > 32768){
			PORTB ^= 0x01; //LED ON
			}else{
			PORTB = 0x00; //LED OFF
		}
	}
	return 0;
}


/** Configuration PORTB as OUTPUT **/
void setupGPIO(){
	DDRB |= 0x01;
	PORTB = 0x00;
}

/**	Configuration of TIMER1 (16Bit) in Nomal mode ,  reset done every 65536 counts **/
void setupTimer(){
	TCCR1A = 0x00; //Disable PIN OC1A (toggle ON/OFF).
	TCCR1B =0x00; //Set Timer in Normal Mode.
	TCCR1B |= (1 << CS11) | (1 << CS10) ; //Config frequency prescaler of timer to 1MHZ/64 = 15625Hz
}


#include <avr/io.h>
#include <util/delay.h>
#define F_CPU 1000000UL

int main(void){
	DDRB |= (1<<PINB2)|(1<<PINB3);/*0b00000100 OUTPUT*/ 
	PORTB=0b00000100;
	_delay_ms(500);
	while(1){
		PORTB ^= (1<<PINB2)|(1<<PINB3);  /*0b00000100 Clignoter PINB2 */ 
		
		_delay_ms(1000);
	}

return 0;
}
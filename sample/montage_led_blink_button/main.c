#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

#define F_CPU 1000000UL

/**Button example
button PC7 will triger a random nunmber to tick every x seconds"**/
void setup();

int main(void){
	setup();
	while(1){
	
		uint8_t randomNumber = (rand() % 9) + 2; //aleatory de 2,3,4,5,6,7,8,9, 10
	
		if(!(PINC & (1 << PINC7))){ //button is pressed when PINC7==0
			for(int i=0;i<randomNumber;i++){
				PORTB ^= (3<<PINB2);  /*0b00000100 Clignoter PINB2 */ 
				_delay_ms(500);
			}
			_delay_ms(1000);
		}else {
			PORTB &= ~((1<<PINB2)|(1<<PINB3));//&0b11110011 
			
		}
	}

return 0;
}

void setup(){

	 srand(12);
	DDRB |= (1<<PINB2)|(1<<PINB3);/*0b00000100 OUTPUT*/ 
	PORTB=0b00000000; /*initial state of pins as output*/
	DDRC &= ~(1<<PINC7); /* 0b01111111&0bxxxxxxxx //PINC7 as INPUT*/
	PORTC=0b10000000; /* SET PINC7 as PULL-UP 5V INPUT IT WILL ALWAYS READ 1 SINCE PULLUP IS ACTIVE*/
	_delay_ms(200);
}
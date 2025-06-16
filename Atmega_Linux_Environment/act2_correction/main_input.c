#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>


/**
This program implements pin PC7 as input connected to a push button, an action will be done when detecting press of the button.
**/
void setup();

int main(void){
	setup();
	while(1) {
		if(!(PINC & (1 << PINC7))) { //Check if the button has been pressed, in other words check if PINC7 == 0V
			PORTB ^= (1 << PINB1) | (1 <<PINB0);   //To blink 2 pins simultaneously (0b00000011) XOR (PORTB)
			_delay_ms(600); //Delay to stabilize button debounce when pressed.
		}
	}
	return 0;
}


/**
To initialize GPIO PINs as either INPUT PULL UP or OUTPUT.
**/
void setup(){
	//Input
	DDRC &= ~(1 << PINC7);
	PORTC = 0b10000000; // Configure PINC7 as input PULL UP (5V if inactive)
	
	//Output
	DDRB |= (1 << PINB1) | (1 << PINB0);
	PORTB &= 0b00000000; //Force all output to zero .
}


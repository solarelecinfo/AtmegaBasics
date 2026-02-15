/*
 * GccApplication4.c
 *
 * Created: 04/05/2024 22:36:42
 * Author : jctor
 */ 

#include <avr/io.h>
#include <avr/delay.h>

#define F_CPU 1000000UL

int main(void)
{
	DDRA|=(1<<PINA0); //output pin PAB0
	PORTA=0x01;
	
	
    /* Replace with your application code */
    while (1) 
    {
		PORTA^=(1<<PINA0)|(1<<PINA7);
		_delay_ms(2000);
		
    }
}


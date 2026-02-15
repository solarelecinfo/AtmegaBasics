#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>

void USART_Init(unsigned int);
void USART_Transmit(unsigned int data);

int main(void)
{
	unsigned int counter = 0;
	//Initialize serial communication
	USART_Init(25);
	while (1)
	{
		USART_Transmit('V');
		USART_Transmit('A');
		USART_Transmit('L');
		USART_Transmit('U');
		USART_Transmit('E');
		USART_Transmit('0'+counter);
		USART_Transmit('\n');
		USART_Transmit('\r');
		_delay_ms(500);
		counter += 1;
		if( counter >= 11){
			counter = 0;
		}
	}
}

/**************************************************************************************
Description:Initialize configuration register for USART interface in asynchronous mode
baud:Integer value for speed baud(bits/s) in UBBR ,example: 2400bauds=25                                                                    
/*************************************************************************************/
void USART_Init (unsigned int baud)
{
	UCSRB = (1 << TXEN);//Enable the receiver usart interface
	UBRRH &= ~(1 << URSEL); //Select register address to write on (UBBRH or UCSRC)
	UBRRH = (unsigned char) (baud >> 8); //Set baud speed (Load MSB bits on UBRR register(bits 11 to 8))
	UBRRL = (unsigned char) baud;//Set baud speed (Load and immediate update LSB bits on UBRR register (bits 7 to 0))
	
	UCSRC = ( 1 << URSEL)|( 1 << UPM1) |(3 << UCSZ0); //Select UCSRC register,set Even parity and Data bit length of 8-bits

}


void USART_Transmit (unsigned int data)
{
//Wait until the Transmitter is ready
	while (! (UCSRA & (1 << UDRE)) ){}; //If UDR buffer is empty(HIGH) then you can load UDR
	UDR = data;//load UDR transmit buffer 
}



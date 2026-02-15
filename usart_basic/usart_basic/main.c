#define F_CPU 1000000UL  
#include <avr/io.h>
#include <avr/delay.h>



void USART_Init(unsigned int);
void USART_Transmit(unsigned int data);

int main(void)
{
unsigned int counter=0;
    // initialiser la communication serial
	USART_Init(25);
    while (1) 
    {
		USART_Transmit('H');
		USART_Transmit('E');
		USART_Transmit('L');
		USART_Transmit('L');
		USART_Transmit('A');
		USART_Transmit(counter);
		USART_Transmit('\n');
		USART_Transmit('\r');
		_delay_ms(500);
		counter+=1;
		if(counter>=250){
			counter=0;
		}
    }
}

/************************************************************************/
/* Description: permet d'initialiser les registres du module USART en mode asychrone
/* baud: valeur entier en ?criture sur les deux registres UBBR correspondant ? la vitesse en baud(bits/s)  
/* example, 2400bauds=25                                                                    */
/************************************************************************/
void USART_Init (unsigned int baud)
{
	//Expliciter l'utilisation du registre UBBRH avant operation d'?criture sur UBBRH
	UBRRH &= ~(1 << URSEL);
	//Partie MSB du registre HAUT (bits 8 to 11)
	UBRRH = (unsigned char) (baud >> 8);
	//Partie LSB du registre BAS (bits 0 ? 7)
	UBRRL = (unsigned char) baud;

	//Enable the receiver and transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);

	//Expliciter l'utilisation du registre UCSRC avant operation d'?criture sur UBBRH
	//Set enable EVEN PARITY
	//Set 1 stop bits and data bit length is 8-bit
	UCSRC = (1<<URSEL)| (1<<UPM1)|(3 << UCSZ0);
}


void USART_Transmit (unsigned int data)
{
	//Wait until the Transmitter is ready
	while (! (UCSRA & (1 << UDRE)) );
	//Get that data outa here!
	UDR = data;
}



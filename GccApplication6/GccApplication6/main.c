/*
 * GccApplication5.c
 *
 * Created: 04/05/2024 00:28:46
 * Author : jctor
 */ 
#define F_CPU 1000000UL  
#include <avr/io.h>
#include <avr/delay.h>



void USART_Init(unsigned int);
void USART_Transmit(char data);

int main(void)
{
    // initialiser la communication serial
	USART_Init(6);
    while (1) 
    {
		USART_Transmit("a");
		_delay_ms(1000);
    }
}

/************************************************************************/
/* Description: permet d'initialiser les registres du module USART en mode asychrone
/* baud: valeur entier en écriture sur les deux registres UBBR correspondant à la vitesse en baud(bits/s)  
/* example, 2400bauds=25                                                                    */
/************************************************************************/
void USART_Init (unsigned int baud)
{
	//Expliciter l'utilisation du registre UBBRH avant operation d'écriture sur UBBRH
	UBRRH &= ~(1 << URSEL);
	//Partie MSB du registre HAUT (bits 8 to 11)
	UBRRH = (unsigned char) (baud >> 8);
	//Partie LSB du registre BAS (bits 0 à 7)
	UBRRL = (unsigned char) baud;

	//Enable the receiver and transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);
	
	//Expliciter l'utilisation du registre UCSRC avant operation d'écriture sur UBBRH
	UCSRC  = (1 << URSEL) ;
	//Set 1 stop bits and data bit length is 8-bit
	UCSRC |= (3 << UCSZ0);
	//Set parity =Even
	UCSRC |= (1<< UPM1);
}


void USART_Transmit (char data)
{
	//Wait until the Transmitter is ready
	while (! (UCSRA & (1 << UDRE)) );

	//Make the 9th bit 0 for the moment
	//UCSRB &=~(1 << TXB8);

	//If the 9th bit of the data is a 1
	//if (data & 0x0100)

	//Set the TXB8 bit to 1
	//UCSRB |= (1 << TXB8);

	//Get that data outa here!
	UDR = data;
}

/**
unsigned char USART_Receive( void )
{
	while ( !(UCSRA & (1 << RXC)) ); //Wait for the RXC to not have 0
	return UDR; //Get that data outa there and back to the main program!
}**/

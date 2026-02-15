#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

volatile unsigned char recieved_char='A';
volatile unsigned char recieved_char_table[4];
volatile int global_index=0;

void USART_Init(unsigned int);
void USART_Transmit(unsigned int data);


int main(void)
{
	unsigned int counter=0;
	// initialiser la communication serial
	USART_Init(25);
	int local_index=0;
	
	while (1)
	{
		USART_Transmit('H');
		USART_Transmit('E');
		USART_Transmit('L');
		USART_Transmit('L');
		USART_Transmit('O');
		USART_Transmit(recieved_char_table[local_index]);
		USART_Transmit('\n');
		USART_Transmit('\r');
		
		counter+=1;
		local_index = ((local_index + 1) % 4);
		if(counter>=10){
			counter=0;
		}
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
//Partie LSB du registre BAS (bits 0 ? 7)
UBRRL = (unsigned char) baud;

//Expliciter l'utilisation du registre UCSRC avant operation d'?criture sur UBBRH
//Set enable EVEN PARITY
//Set 1 stop bits and data bit length is 8-bit
UCSRC = (1<<URSEL)| (1<<UPM1)|(3 << UCSZ0);

//Enable the receiver and transmitter
UCSRB = (1 << RXEN) | (1 << TXEN);
//Enable ADC interrupt de reception
UCSRB |= ( 1 << RXCIE);

//Enable global interrupt
sei();

}


void USART_Transmit_string(char *args){
int nb_elem  = sizeof args / sizeof (char);
for ( int index = 0 ; index < nb_elem ; index++){
USART_Transmit(args[index]);
}

}

void USART_Transmit (unsigned int data)
{
//UCSRB &= ~(1 << RXEN); //desactive le recepteur
//Wait until the Transmitter is ready
while (! (UCSRA & (1 << UDRE)) );
//Get that data outa here!
UDR = data;
//UCSRB |= (1 << TXEN);//activer le recepteur
_delay_ms(200);
}


/**
Interruption lors de la reception d'un caractère a été receptionée par l'interface serial
**/
ISR(USART_RXC_vect) {
// Cette ISR est appelée lorsqu'un octet est reçu en provenande du clavier PC
recieved_char_table[global_index] = UDR;
global_index = ((global_index + 1) % 4);
// Traitez l'octet reçu (stockez-le, envoyez-le sur une interface de communication, etc.)

}

#define F_CPU 1000000UL  
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>


volatile unsigned int increment = 0;
volatile unsigned char recieved_char='A';

void USART_Init(unsigned int);
void USART_Transmit(unsigned int data);
unsigned char USART_Receive(void);

int main(void)
{
unsigned int counter=0;
    // initialiser la communication serial
	USART_Init(25);
	//unsigned char recieved_char='A';
	//Enable compteur TIMER0 interrupt et configuration du comptage ? 1MHZ
	TIMER0_Init();
	
    while (1) 
    {
		USART_Transmit('H');
		USART_Transmit('E');
		USART_Transmit('L');
		USART_Transmit('L');
		USART_Transmit('A');
		USART_Transmit(recieved_char);
		USART_Transmit('\n');
		USART_Transmit('\r');
		_delay_ms(500);
		counter+=1;
		if(counter>=10){
			counter=0;
			//recieved_char=USART_Receive();
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


void USART_Transmit_string(char *args){
	int nb_elem  = sizeof args / sizeof (char);
	for ( int index = 0 ; index < nb_elem ; index++){
		USART_Transmit(args[index]);
	}

}

void USART_Transmit (unsigned int data)
{
	//Wait until the Transmitter is ready
	while (! (UCSRA & (1 << UDRE)) );
	//Get that data outa here!
	UDR = data;
}


/**
Description: method utilisé pour bloquer le thread appelant jusqu'au reception de quelque chose
sur le pin RX
**/
unsigned char USART_Receive(void){
	unsigned char RECEIVE_UDR='N';
	if(UCSRA & (1 << RXC)) {
		RECEIVE_UDR=UDR;
	}else{
		RECEIVE_UDR='N';
	}
	return RECEIVE_UDR;
}


void TIMER0_Init(){
	//PORTC CONFIGURATION SPECIAL PINB3 en sortie OC0(signal du compteur) 
	DDRC =0x01; /* 0b00000001//PINC0 as OUTPUT*/
	PORTC = 0b00000000; /*initial state of pins as output*/
	asm( "nop ");
	
		//TIMER CONFIGURATION
	TCCR0 |= (1<<WGM01); //configurer le mode du timer en CTC valeur max du compteur customisable 
	TCCR0 |= (1<<COM00); //configurer le pin OC0 de seuil de comptage en mode TOGGLE ? chaque MAX_TOP
	TCCR0 |= (1<<CS02)|(1<<CS00); //configurer prescalaire du 8 bit compteur0 en 1MHZ/1024
	
	OCR0 = 244;//MAX_TOP customisable counter value
	
	//INTERRUPT
	sei();
	TIMSK |= (1<<OCIE0);
	TIFR |= (1<<OCF0);
}

ISR(TIMER0_COMP_vect){
	increment++;
	if(increment >4){
		PORTC ^= (1<<PORTC0);
		increment = 0;
		recieved_char=USART_Receive();
	}else{
	
	}

}
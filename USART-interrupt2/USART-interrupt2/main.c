#define F_CPU 1000000UL  
#include <avr/io.h>
#include <stdio.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define VRAI 1
#define FAUX 0
typedef int BOOLEAN;


volatile unsigned int compteur = 0;
volatile BOOLEAN isBlock = FAUX; //VRAI= bloquer FAUX= pas bloquer
volatile int tx_read_index = 0;
volatile int index = 0;
char buffer[10];




int main(void)
{
	int counter=0;
    // initialiser la communication serial
	USART_Init(25);
	//Enable interrupt
    while (1) 
    {
		while( isBlock == VRAI ){}
		isBlock = VRAI; //bloquer le main
		_delay_ms(500);
		sprintf(buffer, "MATHILDE%d\r\n",counter);
		counter=((counter+1)%125);
		UCSRB |= (1 << UDRIE);	//activer interrupption
		
		
    }
}

/************************************************************************/
/* Description: permet d'initialiser les registres du module USART en mode asychrone
/* baud: valeur entier en ?criture sur les deux registres UBBR correspondant ? la vitesse en baud(bits/s)  
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

	//Enable the receiver and transmitter
	UCSRB = (1 << RXEN) | (1 << TXEN);

	//Expliciter l'utilisation du registre UCSRC avant operation d'?criture sur UBBRH
	//Set enable EVEN PARITY
	//Set 1 stop bits and data bit length is 8-bit
	UCSRC = (1<<URSEL)| (1<<UPM1)|(3 << UCSZ0);
	
	//activation d'interruption
	sei();//activation du registre d'interruption global.	
}



//Interruption active lorsque le microcontroleur et pret à transmetre  de données via UDR
ISR(USART_UDRE_vect){
		if(buffer[index] != '\0'){
			UDR = buffer[index];
			index++;
		}else{
			UCSRB &= ~(1 << UDRIE); 	//désactiver l'interruption UDRE
			index = 0;
			isBlock = FAUX;
		}
}


#define F_CPU 1000000UL  
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

volatile unsigned char recieved_char='A';
volatile unsigned char recieved_char_table[4];
volatile int global_index=0;
volatile uint16_t value_adc=0;

void USART_Init(unsigned int);
void USART_Transmit(unsigned int data);
char buffer[20];
unsigned int counter=0;

int main(void)
{

    // initialiser la communication serial
	USART_Init(25);
	ADC_Init();
	 int local_index=0;
	
    while (1) 
    {
		USART_Transmit('H');
		USART_Transmit('E');
		USART_Transmit('L');
		USART_Transmit('L');
		USART_Transmit('A');
		//itoa (counter,buffer,10);
		//float voltage = (float)value_adc * 5.0f / 1024.0f;
        //dtostrf(voltage, 5, 3, buffer);
		itoa (value_adc,buffer,10);
		USART_Transmit_string(buffer);
		USART_Transmit('\n');
		USART_Transmit('\r');
		lecturePinAnalog();
		counter+=1;
		
	
    }
}

/************************************************************************/
/* Description: permet d'initialiser les registres du module USART en mode asychrone
/* baud: valeur entier en ?criture sur les deux registres UBBR correspondant ? la vitesse en baud(bits/s)  
/* example, 2400bauds=25                                                                    */
/************************************************************************/
void ADC_Init ()
{
	//Configurer port en entrée analogique
	DDRC =0x00;
	//Configurer Right-High  MSB du registre de résultat
	ADMUX &= ~(1<<ADLAR);
	//Activer  la reference de voltage en AVCC
	ADMUX |= (1<<REFS0);
	//Configuration frequence selonn intervales 50kHz and 200kHz 
	ADCSRA |= (1<<ADPS1)|(1<<ADPS0); //125kHz avec un prescalaire de 8
	//Activer explicitement  le convertisseur ADC
	ADCSRA |= (1<<ADEN);
	
	// Sélection du pin d'entrée analogique PA1 (ADC1)---explicite
	ADMUX &= 0xE0;
	ADMUX |= 0x01;
}


void lecturePinAnalog(void){
	//Demande explicite d'une conversion
	ADCSRA |= ( 1 << ADSC ) ;
	while ( ADCSRA & (1 << ADSC) ){
		//Attendre que la conversion soit fini	
	}
	//lire le registre de résultat de conversion
	uint8_t theLowADC = ADCL;
	uint16_t theTenBitResults = ADCH<<8 | theLowADC;
	
	value_adc=theTenBitResults;
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

	//Expliciter l'utilisation du registre UCSRC avant operation d'?criture sur UBBRH
	//Set enable EVEN PARITY
	//Set 1 stop bits and data bit length is 8-bit
	UCSRC = (1<<URSEL)| (1<<UPM1)|(3 << UCSZ0);

	//Enable the receiver and transmitter
	UCSRB =  (1 << TXEN);
	//Enable ADC interrupt de reception
	//UCSRB |= ( 1 << RXCIE);

	//Enable global interrupt
	//sei();

}


void USART_Transmit_string(char *args)
{
    while (*args) {
        USART_Transmit(*args++);
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
	_delay_ms(500);
}



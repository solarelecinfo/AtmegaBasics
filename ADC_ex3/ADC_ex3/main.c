#define F_CPU 1000000UL
#include <avr/io.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//Global variables
volatile uint16_t adcValue=0;
char buffer[20];

//Prototype function declaration
void usartInit(unsigned int);
void usartTransmit(unsigned int data);
void adcInit(void)
void readPinAnalog(void)

int main(void)
{
	//Initialize interfaces one time
	usartInit(25);
	adcInit();	
	//Process read on Analog input pin
	while (1)
	{
		usartTransmit('V');
		usartTransmit('A');
		usartTransmit('L');
		usartTransmit(':');
		itoa (adcValue,buffer,10);
		usartTransmitString(buffer);
		usartTransmit('\n');
		usartTransmit('\r');
		readPinAnalog();	
	}
}

/************************************************************************/
/*Description:Initialize ADC interface on single conversion mode(Single Analog Input)                                                                  */
/************************************************************************/
void adcInit()
{
	ADMUX &= ~(1<<ADLAR);//set analog conversion to be right adjusted meaning LSB is at right of the result register
	ADMUX |= (1<<REFS0);//To specify analog reference voltage as AVCC- typically connected to VCC
	//Configuration frequence selonn intervales 50kHz and 200kHz
	ADCSRA |= (1<<ADPS1)|(1<<ADPS0); //125kHz avec un prescalaire de 8
	//Activer explicitement  le convertisseur ADC
	ADCSRA |= (1<<ADEN);
	// Sélection du pin d'entrée analogique PA1 (ADC1)---explicite
	ADMUX &= 0xE0;
	ADMUX |= 0x01;
}

/************************************************************************/
/* Description:Execute punctual conversion on analog input voltage
/************************************************************************/
void readPinAnalog(){
	ADCSRA |= ( 1 << ADSC );//Start conversion explicitly
	while ( ADCSRA & (1 << ADSC) ){} //Wait for conversion to finish
	//Read ADC result register right adjusted
	uint8_t lowAdcValue = ADCL;
	uint16_t tenBitsResult = ADCH<<8 | lowAdcValue;
	adcValue = tenBitsResult;
}

/************************************************************************/
/* Description: Initialize USART asynchronous interface at 2400bauds=25                                                                    */
/************************************************************************/
void usartInit (unsigned int baud)
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


/************************************************************************/
/*Description:To send a String of characters via USART interface one character at a time.                                                                 */
/************************************************************************/
void usartTransmitString(char *args)
{
	while (*args) {
		usartTransmit(*args++);
	}
}

void usartTransmit (unsigned int data)
{
	//UCSRB &= ~(1 << RXEN); //desactive le recepteur
	//Wait until the Transmitter is ready
	while (! (UCSRA & (1 << UDRE)) );
	//Get that data outa here!
	UDR = data;
	//UCSRB |= (1 << TXEN);//activer le recepteur
	_delay_ms(500);
}


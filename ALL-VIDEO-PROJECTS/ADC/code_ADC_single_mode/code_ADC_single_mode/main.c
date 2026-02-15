#define F_CPU 1000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>

//Global variables
volatile uint16_t adcValue=0;
char buffer[20];

//Prototype function declaration
void usartInit(unsigned int);
void usartTransmit(uint8_t);
void usartTransmitString(char *);
void adcInit(void);
void readPinAnalog(void);

int main(void)
{
	//Initialize interfaces one time
	usartInit(25);
	adcInit();
	//Read continuously analog input pin
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

/**********************************************************************************
Description:Initialize ADC interface on single conversion mode(Single Analog Input)
************************************************************************************/
void adcInit()
{
	ADMUX &= ~(1<<ADLAR);//Set analog conversion to be right adjusted meaning LSB is at right of the result register
	ADMUX |= (1<<REFS0);//To specify analog reference voltage as AVCC- typically connected to VCC
	ADMUX |= 0x01;//Set analog input(ADC1) with no Gain
	
	ADCSRA |= (1<<ADPS1)|(1<<ADPS0); //Divide reference 1MHZ frequency by prescalar to set ADC frequency 1MHz/8= 125kHz note: ADC range must be [ 50kHz to 200kHz]
	ADCSRA |= (1<<ADEN);//Enable ADC interface.
}

/************************************************************************
Description:Execute punctual conversion on analog input voltage
************************************************************************/
void readPinAnalog(){
	ADCSRA |= ( 1 << ADSC );//Start conversion explicitly
	while ( ADCSRA & (1 << ADSC) ){} //Wait for conversion to finish
	
	uint8_t lowAdcValue = ADCL; //Read ADC result register(LSB) right adjusted
	uint16_t tenBitsResult = ADCH<<8 | lowAdcValue; //Read remaining part(MSB) of ADC result register (must be read last)
	adcValue = tenBitsResult; //Result ADC 10 bits ADCH(2)-ADCL(8)
}

/************************************************************************
Description: Initialize USART asynchronous interface at 2400bauds=25                                                                    */
/************************************************************************/
void usartInit (unsigned int baud)
{
	UBRRH &= ~(1 << URSEL);//Enable UBBRH write mode
	UBRRH = (unsigned char) (baud >> 8);//Write baud speed on UBRR register MSB(bits 8 to 11)
	UBRRL = (unsigned char) baud;//Write baud speed on UBRR register LSB(bits 0 to 7).
	UCSRC = (1<<URSEL)| (1<<UPM1)|(3 << UCSZ0);//set parity=EVEN, stop_bit=1,data_length=8bits.
	UCSRB =  (1 << TXEN);	//Enable the receiver and transmitter.
}


/****************************************************************************************
Description:To send a String of characters via USART interface one character at a time.
****************************************************************************************/
void usartTransmitString(char *args)
{
	while (*args) {
		usartTransmit(*args++);
	}
}

/************************************************************************
Description: Send data via USART asynchronous interface
************************************************************************/
void usartTransmit (uint8_t data)
{
	while (! (UCSRA & (1 << UDRE)) ){};	//Wait until the Transmitter is ready
	UDR = data;	//Transmit data out
	_delay_ms(500);
}


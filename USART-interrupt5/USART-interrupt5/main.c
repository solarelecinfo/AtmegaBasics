#define F_CPU 1000000UL  
#include <avr/io.h>
#include <stdio.h>
#include <avr/delay.h>
#include <avr/interrupt.h>

#define VRAI 1
#define FAUX 0

typedef int BOOLEAN;

volatile unsigned int increment = 0;
volatile BOOLEAN isMainFlowBlock = FAUX; //VRAI= blocs Data reception ISR on microcontroller.
volatile BOOLEAN isCounterBlock = FAUX; 
volatile int tx_read_index = 0;
volatile int position = 0;
char buffer[30];
int status=0;

void init_usart(unsigned int);
void init_timer0(void);

int main(void)
{
	int counter=0;
	init_usart(25);//Enable USART rate of 2400 baud
	init_timer0();//Enable TIMER0 interrupt with 1MHZ internal clock
    while (1) 
    {
		while( isMainFlowBlock == VRAI || isCounterBlock == VRAI){}
		isMainFlowBlock = VRAI; 
		snprintf(buffer, sizeof(buffer), "count=%d led_status =%d \r\n", counter, status);
		counter = ((counter+1) % 125);
		UCSRB |= (1 << UDRIE);	//active ISR for serial data sending(USART_UDRE_vect)
    }
}

/*******************************************************************************************************/
/* Description: Initializes the registers of the USART module in asynchronous mode.
/* baud: Integer value written to both UBBR registers corresponding to the baud rate (bits per second).
/* Example: 2400 baud = 25                                                               */
/*******************************************************************************************************/
void init_usart(unsigned int baud)
{
	//Select register UBBR before start writing on UBBRH
	UBRRH &= ~(1 << URSEL);
	//MSB(HIGH) Part of register(bits 8 to 11)
	UBRRH = (unsigned char) (baud >> 8);
	//LSB(LOW) Part of register (bits 0 to 7)
	UBRRL = (unsigned char) baud;

	//Enable only transmiter module
	UCSRB = (1 << TXEN) ;

	//Select register UCSR before writing on UBBRH
	//Set Parity=even ,Set stop bit=1 and total data bit length = 8
	UCSRC = (1<<URSEL)| (1<<UPM1)|(3 << UCSZ0);
	
	sei();//activate global interrupts	
}

void init_timer0(){
	DDRC =0x01; /* 0b00000001//PINC0 as OUTPUT*/
	PORTC = 0b00000000; /*initial state of pin as output*/
	
	//TIMER CONFIGURATION
	TCCR0 |= (1<<WGM01); //Configure the timer mode in CTC (Clear Timer on Compare Match), allowing a customizable max count value.
	TCCR0 |= (1<<COM00); //Configure the OC0 pin to toggle on each MAX_TOP value.
	TCCR0 |= (1<<CS02)|(1<<CS00);//Set the prescaler for the 8-bit timer to 1MHz / 1024.
	
	OCR0 = 244;//MAX_TOP counter value
	
	//INTERRUPT
	sei();
	TIMSK |= (1<<OCIE0); //Enable ISR for Timer Compare Match
}


/***************************************************************************************************************/
/* Description: Interruption called whenever DATA is ready to be sent via TX cable and interruption flag was set                                                            */
/***************************************************************************************************************/
ISR(USART_UDRE_vect){
		if( buffer[ position ] != '\0' && position < sizeof(buffer)/sizeof(char) ){
			UDR = buffer[ position ];
			position++;
		}else{
			UCSRB &= ~(1 << UDRIE); 	//Make sure to disable interruption when done sending data(message)
			position = 0;
			isMainFlowBlock = FAUX;
		}
}

/***************************************************************************************************************/
/* Description: Interruption called whenever a compare match(CTC) is reached by timer                                                         */
/***************************************************************************************************************/
ISR(TIMER0_COMP_vect){
	increment++;
	if(increment >4){
		PORTC ^= (1<<PORTC0);
		increment = 0;
		isCounterBlock = FAUX;
		status = PORTC & 0x01;
	}else{
		isCounterBlock = VRAI;
	}
}
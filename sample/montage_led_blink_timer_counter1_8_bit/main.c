#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 1000000UL
volatile unsigned int increment = 0;


void setup();
void setTimer0ModeCTC();
void setExternatInt0Int();

int main(void){
	increment = 0;
	setup();
	while(1){
		
	}
return 0;
}



void setup(){
	//PORTC CONFIGURATION OUTPUT LED 
	//DDRC = 0xFE; /* 0b11111110//PINC1-7=OUTPUT sauf PINC0=INPUT*/
	//PORTC = 0b00000001; /*INPUT PULL UP 5V pour PINC0 */
	DDRC = 0x01; /* 0b11111111//PINC0-7=INPUT*/
	PORTC = 0b11111110; /*INPUT PULL UP 5V pour PORTC SAUD PORT4 TDO INSTABLE  */
	
	asm( "nop ");
	
	setTimer0ModeCTC();
	
	setExternatInt0Int();

	
}

void setTimer0ModeCTC(){
	//CONFIGURER TIMER 0 EN MODE CTCT -PORTB CONFIGURATION SPECIAL PINB3 en sortie OC0(signal du compteur) 
	DDRB |= (1<<PINB3); /* 0b00001000//PINB3(OCO output counter) as OUTPUT*/
	PORTB = 0b00000000;/*No input pull up resistor , consideré comme HIGH-Z undefined voltage  PINB3=output=0*/
	_delay_ms(200);
	
	//TIMER CONFIGURATION
	TCCR0 |= (1<<WGM01); //Configurer mode du timer0 en CTC valeur max du compteur customisable 
	TCCR0 |= (1<<COM00); //Configurer le pin OC0 de seuil de comptage en mode TOGGLE à chaque MAX_TOP=OCR0
	TCCR0 |= (1<<CS02)|(1<<CS00); //configurer prescalaire du 8 bit compteur0 en 1MHZ/1024
	OCR0 = 244;//MAX_TOP compteur customisé au valeur
	
}

void setExternatInt0Int(){
	//INTERRUPT EXTERNAL INT0 on PORTD
	//PORTD CONFIGURATION SPECIAL PINB3 en sortie OC0(signal du compteur) 
	DDRD = 0x00; //PORTD en entrée INPUT
	PORTD = 0xFF; // PULL UP 5 VOLT=HIGH POUR INPUT DE TOUS LES PIND car ils seront lu dans le code

	sei();//activation du registre d'interruption global.
	GICR |= (1<<INT0);//enable Interrupt INT0 dans le SPECIFIC MASK FOR EXTERNAL INTERRUPT REG
	MCUCR |= (1<<ISC01)|(1<<ISC00);//Configurer la patte PD2 en INT0  raising edge + 
}

ISR(INT0_vect){
	increment++;//volatile pour garantir le changement d ela variable avant la condition if 
	if(increment >=2){ //incrementation tous les 250 msecond aproximativement  soit 1/4 de la valeur du compteur du signal OC0
		PORTC ^= (1<<PORTC0);
		increment = 0;
	}
	
}



;
; AssemblerApplication1.asm
;
; Created: 22/07/2024 11:19:06
; Author : jctor
;
;hello.asm
;  turns on an LED which is connected to PB5 (digital out 13)

.include "./m32def.inc"


    .org 0x0000 
	

	ldi r25,0b11000000     ;charger une constant sur registre r25 à servir pour XOR et pour DDRD
	ldi r23,0b10000000   ;charger une constant sur r23 pour l'état initial du PORTD

    out DDRD, r25       ;  Make D.7 et D.6 an output en mettant 0b11000000
	mov r1,r25;    ;XOR constant 
	out PORTD, r23 

start: 
    rcall delay_routine
mark:
	eor r23, r1  
	out PORTD, r23      ; And apply it to the output pin
	mov r1, r23 
	ldi r23,0b11000000
    rjmp start

delay_routine:
		ldi r28,255
		ldi r30,255
		ldi r31,5
loop1: dec r28
       brne loop1    
loop2: dec r30
	   brne loop1
loop3: dec r31
	   brne loop1
	   rjmp mark
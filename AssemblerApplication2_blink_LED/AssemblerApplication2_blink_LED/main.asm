;
; AssemblerApplication1.asm
;
; Created: 22/07/2024 11:19:06
; Author : jctor
;
;hello.asm
;  turns on an LED which is connected to PB5 (digital out 13)

;.include "./m32def.inc"


    .org 0x0000 

	ldi r25,0x02

    ldi r17, (1<<PORTD0)|(1<<PORTD1) ;  Our LED is wired to bit 7 of PortB as OUTPUT=1
    mov r2, r17        ;  R2 will cache the last pattern-to-output
    out DDRD, r2       ;  Make B.7 an output
	; Turn on the LED (initial state)
	ldi r22,0x03       ; Load immediate value into r2 (turn on LED connected to PORTD1)
	mov r1,r25;0b00000010 =PORTDinitial state
	out PORTD, r1


repeat: 
    eor r22, r1        ; Each time around, invert the  "to be driven " LED state 
    out PORTD, r22      ; And apply it to the output pin
	mov r1, r22 
	ldi r22,0x03



	

    ; Even decrementing a 16-bit register pair all the way back around to
    ; zero will only use up 4 * 65536 instruction cycles, which is still
    ; kind of fast for an LED blink rate.   So wrap that delay in one final
    ; loop to repeat it four times for about 1 second at 1MHz CPU clock
    ldi r16, 4
fritter:
        sbiw r30, 1
        brne fritter 
    dec r16
    brne fritter
    rjmp repeat
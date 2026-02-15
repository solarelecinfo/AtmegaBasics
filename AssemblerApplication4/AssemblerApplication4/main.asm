;
; AssemblerApplication4.asm
;
; Created: 25/07/2024 09:37:14
; Author : jctor
;


; Replace with your application code

    .org 0x0000 
start:
	ldi r28,0b10000000
	out DDRA, r28
	out PORTA, r28
	rjmp start

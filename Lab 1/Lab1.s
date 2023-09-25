;****************** main.s ***************
; Program initially written by: Yerraballi and Valvano
; Author: Neha Manda
; Date Created: 1/15/2018 
; Last Modified: 1/19/2023
; Brief description of the program: Solution to Lab1
; The objective of this system is to implement a parity system
; Hardware connections: 
;  One output is positive logic, 1 turns on the LED, 0 turns off the LED
;  Three inputs are positive logic, meaning switch not pressed is 0, pressed is 1
GPIO_PORTD_DATA_R  EQU 0x400073FC
GPIO_PORTD_DIR_R   EQU 0x40007400
GPIO_PORTD_DEN_R   EQU 0x4000751C
GPIO_PORTE_DATA_R  EQU 0x400243FC
GPIO_PORTE_DIR_R   EQU 0x40024400
GPIO_PORTE_DEN_R   EQU 0x4002451C
SYSCTL_RCGCGPIO_R  EQU 0x400FE608
       PRESERVE8 
       AREA   Data, ALIGN=2
; Declare global variables here if needed
; with the SPACE assembly directive
       ALIGN 4
       AREA    |.text|, CODE, READONLY, ALIGN=2
       THUMB
       EXPORT EID
EID    DCB "NM32773",0  ;replace ABC123 with your EID
       EXPORT RunGrader
	   ALIGN 4
RunGrader DCD 1	; change to nonzero when ready for grading
           
      EXPORT  Lab1

Lab1	
 ;initialization
	; turning on clock
	LDR R0, =SYSCTL_RCGCGPIO_R ; load address of clock into R0
	LDR R1, [R0] ; load value of clock register into R1
	ORR R1, #0x10 ; turn on clock for Port E
	STR R1, [R0] ; store back in clock register
	NOP
	NOP ; wait two clock cycles
	
	; DIR: 0 = input, 1 = output
	LDR R0, =GPIO_PORTE_DIR_R
	LDR R1, [R0]
	BIC R1, #0x07 ; clear bits 0, 1, and 2
	ORR R1, #0x10 ; set bit 4
	STR R1, [R0]
	
	;DEN
	LDR R0, =GPIO_PORTE_DEN_R
	LDR R1, [R0]
	ORR R1, #0x17
	STR R1, [R0]
	
	
 
loop
   ;main program loop
   MOV R4, #4
   MOV R5, #5
   MOV R6, #6
   MOV R7, #7
   PUSH {R5,R6}
	ASR R4,R4,R7
	EOR R7,R5,R6
	PUSH {R7,R4}
	POP {R7,R5,R4,R6}
   LDR R0, =GPIO_PORTE_DATA_R ; load R0 with address of data register
   LDR R1, [R0] ; load R1 with contents of data register
   AND R2, R1, #0x01 ; select bit 0 of data register contents
   AND R3, R1, #0x02 ; select bit 1 of data register contents
   LSR R3, R3, #1 ; logical shift right of bit 1
   EOR R2, R2, R3 ; add bit 1 to bit 0
   AND R3, R1, #0x04 ; select bit 2 of data register contents
   LSR R3, R3, #2 ; logical shift right of bit 2
   EOR R2, R2, R3 ; R2 has the number of switches turned on
   EOR R2, R2, #0x01
   LSL R2, #4
   BIC R1, #0x10
   ORR R1, R1, R2
   STR R1, [R0]
   

    B    loop


    
    ALIGN        ; make sure the end of this section is aligned
    END          ; end of file
               
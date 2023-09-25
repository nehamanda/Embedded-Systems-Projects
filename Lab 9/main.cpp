// main.cpp
// Runs on TM4C123
// Modified: 1/2/2023 
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly


// Analog Input connected to PD2=ADC5

// PF3, PF2, PF1 are heartbeats
// Simplex communication
// ECE319K Lab 9 hardware
// Use PC5 38 kHz modulated light output
// System xmt                        System rcv
//   PC5 --IR LED--> light >--IR sensor--- PC4
//   Gnd --------- not connected --------- Gnd

// main1 Test IRtranmission_Init and IRtranmission_OutChar
// final main for Lab 9
//   Import SlidePot and ADC code from Lab8. 
//   Figure out what to do in SysTickHandler (sample, convert, transmit message)

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "PLL.h"
#include "SlidePot.h"
#include "IRtranmission.h"

SlidePot my(150, 0);
uint32_t TxCounter = 0;
uint8_t mail[] = {0x02, 0, 0x2E, 0, 0, 0, 0x03, 0};

extern "C" void DisableInterrupts(void);
extern "C" void EnableInterrupts(void);
extern "C" void TIMER3A_Handler(void);

// PF1 could be toggled in main program
// PF2 could be toggled in SysTick ISR (10 Hz sampling)
#define PF0  (*((volatile uint32_t *)0x40025004))
#define PF1  (*((volatile uint32_t *)0x40025008))
#define PF2  (*((volatile uint32_t *)0x40025010))
#define PF3  (*((volatile uint32_t *)0x40025020))
#define PF4  (*((volatile uint32_t *)0x40025040))

// ***************** Timer3A_Init ****************
// Activate Timer3 interrupts to run user task periodically
// Inputs:  task is a pointer to a user function
//          period in units (1/clockfreq)
//          priority 0 (highest) to 7 (lowest)
// Outputs: none
void Timer3A_Init(uint32_t period, uint32_t priority){
  volatile uint32_t delay;
  SYSCTL_RCGCTIMER_R |= 0x08;   // 0) activate TIMER3
  delay = SYSCTL_RCGCTIMER_R;         // user function
  TIMER3_CTL_R = 0x00000000;    // 1) disable timer2A during setup
  TIMER3_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER3_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER3_TAILR_R = period-1;    // 4) reload value
  TIMER3_TAPR_R = 0;            // 5) bus clock resolution
  TIMER3_ICR_R = 0x00000001;    // 6) clear timer2A timeout flag
  TIMER3_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI8_R = (NVIC_PRI8_R&0x00FFFFFF)|(priority<<29); // priority  
// interrupts enabled in the main program after all devices initialized
// vector number 39, interrupt number 23
  NVIC_EN1_R = 1<<(35-32);      // 9) enable IRQ 35 in NVIC
  TIMER3_CTL_R = 0x00000001;    // 10) enable timer3A
}

void Timer3A_Stop(void){
  NVIC_DIS1_R = 1<<(35-32);   // 9) disable interrupt 35 in NVIC
  TIMER3_CTL_R = 0x00000000;  // 10) disable timer3
}


// Initialize Port F so PF1, PF2 and PF3 are heartbeats
void PortF_Init(void){
  volatile int delay;
  SYSCTL_RCGCGPIO_R |= 0x20;
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTF_DIR_R |= 0x0E;
  GPIO_PORTF_DEN_R |= 0x0E;  
}

// main1 is used to run initial code with IRtranmission
int main1(void){  // run this program and look at signals on PC5
  DisableInterrupts();
  PLL_Init(Bus80MHz);     // set system clock to 80 MHz
  PortF_Init();
  IRtranmission_Init(2375);    // initialize transmitter, baud rate is 2375 bits/sec
  while(1){   
    IRtranmission_OutChar(0x5A);
    for(int i=0;i<100000;i++){};    
  }
}

// final main program for simplex communication
// Sender sends using SysTick Interrupt
int main(void){  // valvano version
  PLL_Init(Bus80MHz);       // Bus clock is 80 MHz 
  ADC_Init();    // initialize to sample ADC
  PortF_Init();
  IRtranmission_Init(2375);    // initialize transmitter, baud rate is 2375 bits/sec
//Enable Timer3 Interrupt by calling SysTick_Init()
  Timer3A_Init(80000000/10,3); // Interrupt at 10Hz
  EnableInterrupts();
  while(1){ // runs every 100 ms
    PF1 ^= 0x02;  // Heartbeat
  }
}
/*
void sendData(int m0, int m1, int m2, int m3) {
	uint8_t sum;
	IRtranmission_OutChar(STX);
	sum = STX;
	IRtranmission_OutChar(m0);
	sum += m0;
	IRtranmission_OutChar('.');
	sum += '.';
	IRtranmission_OutChar(m1);
	sum += m1;
	IRtranmission_OutChar(m2);
	sum += m2;
	IRtranmission_OutChar(m3);
	sum += m3;
	IRtranmission_OutChar(ETX);
	sum += ETX;
	IRtranmission_OutChar(sum);
}

void fix(int n) {
	int m0, m1, m2, m3;
	m0 = (n/1000)%10 + 0x30;
	m1 = (n/100)%10 + 0x30;
	m2 = (n/10)%10 + 0x30;
	m3 = n%10 + 0x30;
	sendData(m0, m1, m2, m3);
}
*/
void TIMER3A_Handler(void){
  TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
//Similar to Lab8 except rather than grab sample, and put in mailbox
// form a message, transmit 

  PF2 ^= 0x04;  // Heartbeat
	uint32_t adc_in = ADC_In();
	my.Save(adc_in);
	PF2 ^= 0x04;
	PF2 ^= 0x04;
	uint32_t d = my.Distance();
	//fix(distance);
  PF2 ^= 0x04;  // Heartbeat
	
	mail[1] = (d/1000)%10 + 0x30;
	mail[3] = (d/100)%10 + 0x30;
	mail[4] = (d/10)%10 + 0x30;
	mail[5] = d%10 + 0x30;
	mail[7] = mail[0] + mail[1] + mail[2] + mail[3] + mail[4] + mail[5] + mail[6];
	for(int i = 0; i < 8; i++) {
		IRtranmission_OutChar(mail[i]);
	}
	
	TxCounter++;
  // you write this
  PF2 ^= 0x04;  // Heartbeat
}


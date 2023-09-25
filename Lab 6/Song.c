// Song.c
// Lab 6 Extra credit 1) 
// playing your favorite song.
//
// For use with the TM4C123
// ECE319K lab6 extra credit 1)
// Program written by: put your names here
// 1/2/23

#include "Sound.h"
#include "Song.h"
#include "../inc/DAC.h"
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
typedef const struct note note_t;

/* note_t rickroll[34]={
{C0, s},
{D, s},
{F, s},
{D, s},
{A, de},
{Q, g},
{A, de},
{G, dq},
{C0, s},
{D, s},
{F, s},
{D, s},
{G, de},
{Q, g},
{G, de},
{F, de},
{E, s},
{D, e},
{C0, s},
{D, s},
{F, s},
{D, s},
{F, q},
{G, e},
{E, de},
{D, s},
{C0, e},
{Q, g},
{C0, e},
{Q, g},
{C0, e},
{G, q},
{F, h},
{0,0}
};
*/

note_t tangled[] = {
	{G, q},
	{F, q},
	{E, h},
	{D, e},
	{E, q},
	{D, s},
	{B0, s},
	{C0, e},
	{G0, e},
	{Q, q},
	{G, q},
	{F, q},
	{E, h},
	{D, e},
	{E, q},
	{D, s},
	{B0, s},
	{C0, q},
	{Q, q},
	{A, q},
	{G, q},
	{F, h},
	{G, e},
	{C, e},
	{G, e},
	{C0, e},
	{E, q},
	{D, q},
	{G, q},
	{Q, e},
	{F, e},
	{E, q},
	{C0, q},
	{D, dh},
	{Q, q},
	{A, q},
	{G, q},
	{F, q},
	{Q, e},
	{C0, e},
	{B, q},
	{A, q},
	{G, q},
	{Q, e},
	{C0, e},
	{C, dq},
	{B, e},
	{A, dq},
	{E, e},
	{G, h},
	{Q, q},
	{A, e},
	{B, e},
	{C, q},
	{F, q-g},
	//{Q, g},
	{F, dq},
	{E, e},
	{G, h},
	{Q, q},
	{C0, e},
	{D, e-g},
	//{Q, g},
	{D, dq-g},
	//{Q, g},
	{D, e},
	{G, dq},
	{F, e-g},
	//{Q, g},
	{F, q},
	{E, q},
	{Q, q},
	{A, e},
	{B, e},
	{C, q},
	{F, q-g},
	//{Q, g},
	{F, dq},
	{E, e},
	{G, h},
	{Q, q},
	{C0, e},
	{D, e},
	{E, dq},
	{F, e},
	{E, dq},
	{D, e},
	{C0, h},
	{Q, q},
	{0, 0}
	
};

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

void Timer0A_Init(uint32_t period) {
	volatile int delay;
	SYSCTL_RCGCTIMER_R |= 0x01;
	delay = 0;
	TIMER0_CTL_R = 0x00000000;
	TIMER0_CFG_R = 0x00000000;
	TIMER0_TAMR_R = 0x00000002;
	TIMER0_TAILR_R = period - 1;
	TIMER0_TAPR_R = 0;
	TIMER0_ICR_R = 0x00000001;
	TIMER0_IMR_R = 0x00000001;
	NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF) | 0x80000000;
	NVIC_EN0_R = 1<<19;
	TIMER0_CTL_R = 0x00000001;
}
note_t *songPt;

void Timer0A_Handler(void) {
	TIMER0_ICR_R = TIMER_ICR_TATOCINT;
	if ((songPt == 0) || (songPt -> duration) == 0) {
		TIMER0_CTL_R = 0x00000000;
		TIMER0_IMR_R = 0x00000000;
	}
	else {
		Sound_Start(songPt -> reloadval);
		TIMER0_TAILR_R = songPt -> duration;
		songPt++;
	}
	
	
}


void Song_Init(void){
  // extra credit 1)
	Timer0A_Init(bus);
	TIMER0_IMR_R = 0x00000000;
	songPt = 0;

}

// Play song, while button pushed or until end
void Song_Play(void){
  // extra credit 1)
	songPt = tangled;
	TIMER0_IMR_R = 0x00000001;

}

// Stop song
void Song_Stop(void){
 // if needed
}


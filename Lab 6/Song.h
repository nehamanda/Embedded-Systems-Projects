// Song.h
// Lab 6 Extra credit 1) 
// playing your favorite song.
//
// For use with the TM4C123
// ECE319K lab6 extra credit
// Program written by: put your names here
// 1/2/23


// Description: 
// This file contains the interrupt delays for notes, the samples
// for the sine wave used to make sound, and the data structure
// containing notes for your favorite.
//
#include "Sound.h"
#ifndef SONG_H
#define SONG_H
#define bus 80000000

#define q (bus/4)

#define dq (1.5*q)

#define e (q/2)

#define s (e/2)

#define de (1.5*e)

#define h (2*q)

#define dh (1.5*h)

#define g (q/10000)

struct note {
	int reloadval;
	int duration;
};


void Song_Init(void);
// Play your favorite song, while button pushed or until end
void Song_Play(void);

// Stop song
void Song_Stop(void);

#endif


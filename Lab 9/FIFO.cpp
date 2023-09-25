// FIFO.cpp
// Runs on any microcontroller
// Provide functions that initialize a FIFO, put data in, get data out,
// and return the current size.  The file includes a transmit FIFO
// using index implementation and a receive FIFO using pointer
// implementation.  Other index or pointer implementation FIFOs can be
// created using the macros supplied at the end of the file.
// Modified: 2/2/2022
// Student names: change this to your names or look very silly
// Last modification date: change this to the last modification date or look very silly


#include <stdint.h>

#include "FIFO.h"
#include "ST7735.h"
//#include "print.h"
uint8_t size = 9;
char fifo[8];

// A class named Queue that defines a FIFO
Queue::Queue(){
  // Constructor - make FIFO initially empty
  // write this
	
	uint8_t PutI = 0;
	uint8_t GetI = 0;
}

// To check whether Queue is empty or not
bool Queue::IsEmpty(void){
	if (PutI == GetI) {
		return true;
	}
  // write this
  return false; // replace this line
}

  // To check whether Queue is full or not
bool Queue::IsFull(void){
	if (((PutI + 1)%size) == GetI) {
		return true;
	}
  // write this
  return false; // replace this line
}

  // Inserts an element in queue at rear end
bool Queue::Put(char x){
  // write this
	if (Queue::IsFull()) {
		return false;
	}
	fifo[PutI] = x;
	PutI = (PutI + 1)%size;
	return true;
   // replace this line
}

  // Removes an element in Queue from front end. 
bool Queue::Get(char *pt){
  // write this
  return false; // replace this line
}

  /* 
     Printing the elements in queue from front to rear. 
     This function is only to test the code. 
     This is not a standard function for Queue implementation. 
  */
void Queue::Print(void){
  // write this

}



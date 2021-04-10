// bits.c ... functions on bit-strings
// part of signature indexed files
// Bit-strings are arbitrarily long byte arrays
// Least significant bits (LSB) are in array[0]
// Most significant bits (MSB) are in array[nbytes-1]

// Written by John Shepherd, March 2019

#include <assert.h>
#include "defs.h"
#include "bits.h"
#include "page.h"

typedef struct _BitsRep {
	Count  nbits;		  // how many bits
	Count  nbytes;		  // how many bytes in array
	Byte   bitstring[1];  // array of bytes to hold bits
	                      // actual array size is nbytes
} BitsRep;

// create a new Bits object

Bits newBits(int nbits)
{
	Count nbytes = iceil(nbits,8);
	Bits new = malloc(2*sizeof(Count) + nbytes);
	new->nbits = nbits;
	new->nbytes = nbytes;
	memset(&(new->bitstring[0]), 0, nbytes);
	return new;
}

// release memory associated with a Bits object

void freeBits(Bits b)
{
	free(b);
}

// check if the bit at position is 1

Bool bitIsSet(Bits b, int position)
{
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	int set = position % 8;
	int place = position/8;
	Byte buffer = (1<<set);
	if(b->bitstring[place] && buffer){
		return TRUE;
	}
	return FALSE; // remove this
}

// check whether one Bits b1 is a subset of Bits b2

Bool isSubset(Bits b1, Bits b2)
{
	assert(b1 != NULL && b2 != NULL);
	assert(b1->nbytes == b2->nbytes);
	//TODO
	int checkequal = 0;
	for(int i = 0 ; i < b1->nbytes ; i++){
		if ((b1->bitstring[i] & b2->bitstring[i]) != b1->bitstring[i]) {
			checkequal = 1;
			break;
		}
	}
	if(checkequal == 0){
		return TRUE;
	}
	return FALSE; // remove this
}

// set the bit at position to 1

void setBit(Bits b, int position)
{
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	int set = position % 8;
	int place = position/8;
	Byte buffer = (1<<set);
	b->bitstring[place] |= buffer;	
	//TODO
}

// set all bits to 1

void setAllBits(Bits b)
{
	assert(b != NULL);

	for(int i = 0; i<b->nbytes;i++){
		b->bitstring[i] |= 0xFF;
	}
	//TODO
}

// set the bit at position to 0

void unsetBit(Bits b, int position)
{
	assert(b != NULL);
	assert(0 <= position && position < b->nbits);
	int set = position % 8;
	int place = position/8;
	Byte buffer = ~(1<<set);
	b->bitstring[place] &= buffer;	
	//TODO
}

// set all bits to 0

void unsetAllBits(Bits b)
{
	assert(b != NULL);
	for(int i = 0; i<b->nbytes;i++){
		b->bitstring[i] &= 0x00;
	}
	//TODO
}

// bitwise AND ... b1 = b1 & b2

void andBits(Bits b1, Bits b2)
{
	assert(b1 != NULL && b2 != NULL);
	assert(b1->nbytes == b2->nbytes);
	for(int i = 0; i < b1->nbytes;i++){
		b1->bitstring[i] &= b2->bitstring[i];
	}
	//TODO
}

// bitwise OR ... b1 = b1 | b2

void orBits(Bits b1, Bits b2)
{
	assert(b1 != NULL && b2 != NULL);
	assert(b1->nbytes == b2->nbytes);
	for(int i = 0; i<b1->nbytes;i++){
		b1->bitstring[i] |= b2->bitstring[i];
	}
	//TODO
}

// left-shift ... b1 = b1 << n
// negative n gives right shift

void shiftBits(Bits b, int n)
{
	if(n > 0){
		int num = n / 8;
		int num1 = n % 8;
		if(num !=  0){
			for(int i = 0 ; i < b->nbytes;i++){
				if(i < b->nbytes - num){
					b->bitstring[i] = b->bitstring[i+num];
				}else{
					b->bitstring[i] = 0x00;
				}
			}
		}
		for(int i = 0 ; i < b->nbytes-num-1;i++){
			Byte buffer = num1 << b->bitstring[i];
			Byte buffer2 = (8 - num1) >> b->bitstring[i+1];
			buffer &= buffer2;
			b->bitstring[i] = buffer;
		}
		b->bitstring[b->nbytes-num] = num1 << b->bitstring[b->nbytes-num];
	}else{
		int num = n / 8;
		int num1 = n % 8;
		if(num !=  0){
			for(int i = b->nbytes-1 ; i >= 0 ;i--){
				if(i > 0){
					b->bitstring[i] = b->bitstring[i-num];
				}else{
					b->bitstring[i] = 0x00;
				}
			}
		}
		for(int i = b->nbytes-num - 1; i > 0;i--){
			Byte buffer = num1 >> b->bitstring[i];
			Byte buffer2 = (8 - num1) << b->bitstring[i-1];
			buffer &= buffer2;
			b->bitstring[i] = buffer;
		}
		b->bitstring[0] = num1 >> b->bitstring[0];
	}
    // TODO
}

// get a bit-string (of length b->nbytes)
// from specified position in Page buffer
// and place it in a BitsRep structure

void getBits(Page p, Offset pos, Bits b)
{
	Byte *getpage = addrInPage(p,pos,b->nbytes);
	memcpy(b->bitstring,getpage,b->nbytes);
	//TODO
}

// copy the bit-string array in a BitsRep
// structure to specified position in Page buffer

void putBits(Page p, Offset pos, Bits b)
{
	Byte *getpage = addrInPage(p,pos,b->nbytes);
	memcpy(getpage,b->bitstring,b->nbytes);
	//TODO
}

// show Bits on stdout
// display in order MSB to LSB
// do not append '\n'

void showBits(Bits b)
{
	assert(b != NULL);
    //printf("(%d,%d)",b->nbits,b->nbytes);
	for (int i = b->nbytes-1; i >= 0; i--) {
		for (int j = 7; j >= 0; j--) {
			Byte mask = (1 << j);
			if (b->bitstring[i] & mask)
				putchar('1');
			else
				putchar('0');
		}
	}
}

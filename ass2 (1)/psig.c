// psig.c ... functions on page signatures (psig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "psig.h"
#include "hash.h"
Bits pcodeword(char *attr_value, int m, int k);

Bits pcodeword(char *attr_value, int m, int k)
{
   int  nbits = 0;   // count of set bits
   Bits cword = newBits(m);   // assuming m <= 32 bits
   srandom(hash_any(attr_value,strlen(attr_value)));
   while (nbits < k) {
      int i = random() % m;
      if (!bitIsSet(cword,i)) {
         setBit(cword,i);
         nbits++;
      }
   }
   return cword;  // m-bits with k 1-bits and m-k 0-bits
}
Bits makePageSig(Reln r, Tuple t)
{
	assert(r != NULL && t != NULL);
	//TODO
	Bits newbits = newBits(psigBits(r));
	char **values = tupleVals(r,t);
	for(int i = 0; i < nAttrs(r);i++){
		if (strcmp(values[i], "?") != 0) {
			orBits(newbits,pcodeword(values[i],psigBits(r),codeBits(r)));
		}
	}
	return newbits;
	
	return NULL; // remove this
}

void findPagesUsingPageSigs(Query q)
{
	assert(q != NULL);
	//TODO
	setAllBits(q->pages); // remove this
}


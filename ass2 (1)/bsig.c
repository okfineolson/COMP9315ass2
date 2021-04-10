// bsig.c ... functions on Tuple Signatures (bsig's)
// part of signature indexed files
// Written by John Shepherd, March 2019

#include "defs.h"
#include "reln.h"
#include "query.h"
#include "bsig.h"
#include "psig.h"
void findPagesUsingBitSlices(Query q)
{
	Reln r = q->rel;
	assert(q != NULL);
	Bits Qsig = makePageSig(r,q->qstring);
	for(int i = 0; i < psigBits(r); i++){
		if(bitIsSet(Qsig,i)){
			PageID ppid = i / maxBsigsPP(r);
			Page Slice = getPage(bsigFile(r),ppid);
			q->nsigpages++;
			Bits newbit = newBits(bsigBits(r));
			Count num = i % maxBsigsPP(r);//or offset num ?
			getBits(Slice,num,newbit);
			for (int j = 0; j < nPsigs(r); j++)
			{
				if(bitIsSet(newbit, j) == FALSE){
					unsetBit(q->pages, j);
				}
			}
			q->nsigs++;
		}
	}
	//TODO
	/*
    Qsig = makePageSig(Query)
	Pages = AllOneBits
	for each i in 0..pm-1 {
		if (Qsig bit[i] is 1) {
			Slice = get i'th bit slice from bsigFile
			zero bits in Pages which are zero in Slice
		}
	}
	*/
	setAllBits(q->pages); // remove this
}


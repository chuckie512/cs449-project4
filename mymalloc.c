#include "malloc.h"


//the beginning and end of the allocated heap
static void * beginning = NULL;
static void * end = NULL;


void * mymalloc(int size){
	
	
	if(size%2!=0)	//size must be even in this implementation, as the bit used for 1 will be re-purposed.
		size++;
	int data = size|1;
	
	
	if(beginning = NULL){	//the beginning has not yet be initialized 
		beginning = sbrk(0);	//sets the beginning pointer to the brk;
		end = sbrk(2*sizeof(int)+size);	//allocates 2 extra sizeof(int) for the data
		*(int*) beginning = data;	//adds the data to the header/footer
		*(int*) (end-1) = data;
		return ((int*)beginning)+1;	//returns the pointer if it is the first run
	}
	
	
	//ok, we're initialized!!
	//now to see if we have size open space on the heap.
	
	
	void* current = beginning;	//sets the comparison to the first block
	
	while(current < end){
		
		int currentData = *(int*) current;
		int currentBlockSize = currentData & 0;
		
		
		if(!currentData & 1){	//current block is free
			
			
			if(size<=currentBlockSize){	//data can fit here!
				int * header = (int*) current;
				void* payloadStart = (void*) (((int*)current) + 1);	// the value to be returned 
				
				//increment current to start of footerData
				current = ((int*)current) + 1;
				current+=size;
				int* footer = (int*)(payloadStart + size);
				int* currentFooter = (int*)(payloadStart + currentBlockSize);
				
				if(currentFooter-footer < 2*sizeof(int)){	//Not enough room to squeeze in another block, so we're gonna waste some space here
					size = currentBlockSize;
					data = int data = size|1
					*header = data;
					*currentFooter = data;
					return payloadStart;	//returns the start of the payload 
				}
				
				//if we're here we have enough space for another block here.
				*header = data;
				*footer = data;
				//the current block is done, but now we have to deal with the extra space
				int* newHeader = footer+1;
				int newSize = currentBlockSize - size - 2*sizeof(int);
				data = newSize; //last bit should be 0, so it's set to free
				//adds the data to the new area
				*newHeader = data;
				*currentFooter = data;
				return payloadStart; // returns the payload address
				
			}
		}
		//if we're here, the current block wasn't free, let's iterate to the next one.
		current += currentBlockSize + 2*sizeof(int);
	}
	
	//if we're here, then we made it to the end of the heap without finding enough free space.
	//that means we must allocate some more!
	int* header = end;	//the header must be at end...
	void* payloadStart = (void*) (header+1);	//payload start is after the data
	int* footer = (int*) (payloadStart+size);	//footer starts at the end
	brk(footer+1); //allocate enough room for all data and payload
	*header = data;
	*footer = data;
	return payloadStart;	//return the pointer
	

}

void myfree(void *ptr){
	

}

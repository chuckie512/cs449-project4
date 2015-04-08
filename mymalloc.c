#include "malloc.h"


//the beginning and end of the allocated heap
static void * beginning = NULL;
static void * end = NULL;


void * mymalloc(int size){
	
	
	if(size%2!=0)	//size must be even in this implementation, as the bit used for 1 will be re-purposed.
		size++;
	int data = size|1;
	
	
	if(beginning == NULL){	//the beginning has not yet be initialized 
		
		//printf("first run\n");
		
		beginning = sbrk(0);	//sets the beginning pointer to the brk;
		sbrk(2*sizeof(int)+size);	//allocates 2 extra sizeof(int) for the data
		end = sbrk(0);
		//printf("ending ptr after first malloc: %p\n",end);
		*(int*) beginning = data;	//adds the data to the header/footer
		//printf("header data for first ptr: %d\n",*(int*) beginning);
		*((int*) (end)-1) = data;
		//printf("footer for first ptr: %p\n",((int*) (end)-1));
		return ((int*)beginning)+1;	//returns the pointer if it is the first run
	}
	
	
	//ok, we're initialized!!
	//now to see if we have size open space on the heap.
	
	
	void* current = beginning;	//sets the comparison to the first block
	
	while(current < end){
		
		int currentData = *(int*) current;
		int currentBlockSize = currentData & 0xFFFFFFFE;
		
		//printf("header at: %p has data of %d\n",current,currentData);
		
		
		if(!(currentData & 1)){	//current block is free
			//printf("current block is free\n");
			
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
					data = size|1;
					*header = data;
					*currentFooter = data;
					//printf("special case, not enough room to split\n");
					//printf("size of new block: %d\n",size);
					//printf("data for new block: %d\n",data);
					//printf("payload void*: %p\n", payloadStart);
					//printf("header addr: %p\n", header);
					//printf("footer addr: %p\n", currentFooter);
					return payloadStart;	//returns the start of the payload 
				}
				
				//if we're here we have enough space for another block here.
				*header = data;
				*footer = data;
				//printf("data for new block: %d\n",data);
				//the current block is done, but now we have to deal with the extra space
				int* newHeader = footer+1;
				int newSize = currentBlockSize - size - 2*sizeof(int);
				data = newSize; //last bit should be 0, so it's set to free
				//adds the data to the new area
				*newHeader = data;
				*currentFooter = data;
				
				//printf("size of new block: %d\n",size);
				
				//printf("payload void*: %p\n", payloadStart);
				//printf("header addr: %p\n", header);
				//printf("footer addr: %p\n", footer);
				//printf("header val: %d\n", *header);
				//printf("footer val: %d\n",*footer);
				return payloadStart; // returns the payload address
				
			}
		}
		//printf("current block is in use\n");
		//if we're here, the current block wasn't free, let's iterate to the next one.
		current += currentBlockSize + 2*sizeof(int);
	}
	
	//printf("adding to end\n");
	//printf("end ptr: %p\n",end);
	
	//if we're here, then we made it to the end of the heap without finding enough free space.
	//that means we must allocate some more!
	int* header = end;	//the header must be at end...
	void* payloadStart = (void*) (header+1);	//payload start is after the data
	int* footer = (int*) (payloadStart+size);	//footer starts at the end
	brk((void*)(footer+1)); //allocate enough room for all data and payload
	*header = data;
	*footer = data;
	end = sbrk(0); //updating the end pointer
	
	//printf("size of new block: %d\n",size);
	//printf("data for new block: %d\n",data);
	//printf("payload void*: %p\n", payloadStart);
	//printf("header addr: %p\n", header);
	//printf("footer addr: %p\n", footer);
	//printf("header val: %d\n", *header);
	//printf("footer val: %d\n",*footer);
	return payloadStart;	//return the pointer
	

}

void myfree(void *ptr){

	
	int* header = ((int*) (ptr))-1;
	if(*header%2==0){
		//printf("already freed!\n");
		return;
	}
	if(header<beginning||header>end){
		//printf("not in range!\n");
		return;
	}
	//printf("old value of header of freed data: %d\n", *header);
	int size = *header -1;
	int* footer = (int*) (ptr + size);
	*header = *header-1;
	*footer = *footer-1;
	//printf("header of freed data: %p\n",header);
	//printf("footer of freed data: %p\n",footer);
	//printf("new value of freed footer: %d\n",*footer);
	
	int* prevFooter = header-1;
	int* nextHeader = footer+1;
	if(prevFooter>beginning){
		if(!(*prevFooter &1 )){ //previous block is also free, time to make a bigger block
			//printf("prev block is free, consolidating into 1 block\n");
			int prevSize = *prevFooter;
			void* endPrevPayload = ((void*)prevFooter)-1;
			void* startPrevPayload = endPrevPayload-prevSize+1; //THIS +1 TOOK ME AN HOUR AHHHHHHHHHHHHHHHH
			//printf("prev block payload start %p\n", startPrevPayload);
			int* prevHeader = ((int*)startPrevPayload)-1;
			*prevHeader=*header+2*sizeof(int)+prevSize; //new value for header
			
			header = prevHeader; //updates pointers
			*footer = *header;  //updates footer value
			//printf("new free block size: %d\n",*header);
		}
	}
	
	if(nextHeader<end){
		if(!(*nextHeader&1 )){ //nextblock is free
			//printf("next block is free, consolidating into1 block\n");
			int nextSize = *nextHeader;
			void* startNextPayload = (void*) (nextHeader+1);
			void* endNextPayload = startNextPayload+nextSize-1;  //THIS -1 TOOK ME AN HOUR AHHHHHHHHHHHHHHHH
			int* nextFooter = (int*)(endNextPayload+1);
			*nextFooter = *header+2*sizeof(int)+nextSize;
			footer = nextFooter;
			*header = *footer;
			//printf("new free block size %d\n",*header);
			//printf("new footer %p\n", footer);
			
			
		}
	}
	if( ((void*)(footer+1))+1 >= end){ //this is the last block! let's shrink
		
		end = (void*)header;
		//printf("shrinking to brk: %p\n",end);
		brk(end);
		//printf("we shrunk!\nbrk: %p\n",sbrk(0));
		return;
	}

}

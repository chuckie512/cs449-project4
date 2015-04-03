#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

//include your code
#include "mymalloc.h"


int main(){
	printf("sizeof(int) %d\n",sizeof(int));
	printf("original val brk: %p\n", sbrk(0));
	printf("\n");
	void* ptr = mymalloc(0xFF);
	printf("1st val brk: %p\n", sbrk(0));
	printf("\n");
	mymalloc(0x2);
	printf("2nd val brk: %p\n", sbrk(0));
	printf("\n");
	printf("freeing mem at: %p\n",ptr);
	myfree(ptr);
	printf("\n");
	ptr = mymalloc(0xAA);
	printf("new mem alloc at: %p\n", ptr);
	printf("3rd val brk: %p\n", sbrk(0));
	printf("\n");
	ptr = mymalloc(0x4);
	printf("new mem alloc at: %p\n", ptr);
	printf("4th val brk: %p\n", sbrk(0));
	
}
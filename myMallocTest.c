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
	printf("ptr1\n");
	void* ptr1 = mymalloc(0xFF);
	printf("1st val brk: %p\n", sbrk(0));
	printf("\n");
	printf("ptr2\n");
	void* ptr2 = mymalloc(0x2);
	printf("2nd val brk: %p\n", sbrk(0));
	printf("\n");
	printf("ptr1\n");
	printf("freeing mem at: %p\n",ptr1);
	myfree(ptr1);
	printf("\n");
	printf("ptr3\n");
	void* ptr3 = mymalloc(0xAA);
	printf("new mem alloc at: %p\n", ptr3);
	printf("3rd val brk: %p\n", sbrk(0));
	printf("\n");
	printf("ptr4\n");
	void* ptr4 = mymalloc(0x4);
	printf("new mem alloc at: %p\n", ptr4);
	printf("4th val brk: %p\n", sbrk(0));
	printf("\n");
	printf("ptr2\n");
	myfree(ptr2);
	printf("\n");
	printf("ptr3\n");
	myfree(ptr3);
	printf("\n");
	printf("ptr4\n");
	myfree(ptr4);
	printf("\n");
	printf("final brk %p\n",sbrk(0));
	
}
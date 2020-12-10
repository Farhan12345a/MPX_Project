#ifndef MEMORY_MANAGMENT_H 
#define MEMORY_MANAGMENT_H
#include <system.h>
#include <mem/heap.h>

#define HEAP_SIZE 50000
#define FREE_MEMORY 1
#define ALLOCATED_MEMORY 2
//complete memory control block 
typedef struct CMCB_s{ 
	int type;
	u32int start;
	int size;
	char namePCB[9];
	//pointer to next free block of memory
	struct CMCB_s* next;
	//pointer to prev free block of memory
	struct CMCB_s* prev;
}CMCB_t; 
//contains type of memory and size and is used to keep memory block sized. 
typedef struct LMCB_s{
	int type;
	int size;
}LMCB_t; 

/**
* Function: heapIsEmpty
* -------------------------------
* Searches the memory list and returns 
* 1 if no allocated blocks are found in it.
* @ param int size: size of desired heap
* @ return int: returns 1 if empty 0 otherwise
* @author Brendan Michael
*/
int heapIsEmpty();

/**
* Function: initializeHeap
* -------------------------------
* Initializes the heap that stores memory
* blocks. Inserts a single CMCB that represents
* the free memory allocated for the heap.
* @ param int size: size of desired heap
* @ return int: returns 1 if sucessful 0 otherwise
* @author Brendan Michael
*/
int initializeHeap(int size);
/**
* Function: merge
* -------------------------------
* Merges the just freed memory block with 
* adjacent free memory blocks. Merged blocks
* are removed from the memory list.
* @ param CMCB_t *current: the just freed memory block to be merged
* @author Brendan Michael
*/
void merge(CMCB_t *current);
/**
* Function: allocateMemory
* -------------------------------
* Allocates a memory block of the desired size.
* Uses the first fit strategy, the first first free
* memory block with a large enough size is the one
* allocated.
* @param u32int bytes: The requested block size
* @return u32int: returns the address of the allocated block
* @author Brendan Michael
*/
u32int allocateMemory(u32int bytes);
/**
* Function: freeMem
* -------------------------------
* Frees the memory at a given address.
* Calls merge to combine the freed block
* with adjacent free blocks. 
* @param void *ptr: Memory address of block to be freed
* @return int: returns 1 is sucessful 0 otherwise
* @author Brendan Michael
*/
int freeMem(void *ptr);
void printCMCB(CMCB_t* block);
void printAllocated();
void printFree();
void createmList();
void printAll();
#endif

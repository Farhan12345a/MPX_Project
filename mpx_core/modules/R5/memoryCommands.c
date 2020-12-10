#include "memoryManagment.h" 
#include "memoryCommands.h"
#include "../mpx_supt.h"
#include <string.h>
#include "mList.h"

/**
 * Function: allocateMemoryCall
 * -----------------------
 * Prompts the user  for how
 * much memory they will like to allocate,
 * in bytes, and allocates it using the
 * allocateMemory function(first fit)
 * 
 * @authors Farhan Shahbaz
 */
void allocateMemoryCall(){
    printlnMessage("How much memory would you like to allocate?");
    char buffer[10];
    int size = 10;
    memset(buffer,0,10*sizeof(char));
    sys_req(READ, DEFAULT_DEVICE, buffer, &size);
    allocateMemory(atoi(buffer));

}

/**
 * Function: freeMemoryCall
 * -----------------------
 * Prompts the user for which
 * memory address they would like to 
 * free (has to be allocated), and frees
 * it using the freeMem function
 * 
 * @authors Farhan Shahbaz
 */
void FreeMemoryCall(){
    printlnMessage("Which memory address would you like to free?");
    char buffer[10];
    int size = 10;
    memset(buffer,0,10*sizeof(char));
    sys_req(READ, DEFAULT_DEVICE, buffer, &size);
    freeMem((void*)atoi(buffer));
}

/**
 * Function: isEmptyCall
 * -----------------------
 * Informs the user if the heap is empty
 * 
 * @authors Selim Demircan
 */
void isEmptyCall(){
    if(heapIsEmpty()){
        printlnMessage("Heap is Empty");
    }
    else{
        printlnMessage("Heap is not Empty");
    }
}


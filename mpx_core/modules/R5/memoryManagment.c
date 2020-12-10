#include "memoryManagment.h"
#include "../mpx_supt.h"
#include <string.h>
#include "mList.h"

mList *MCBList; 
void *heap_top = NULL;

/**
 * Function: createmList
 * -----------------------
 * Initializes MCBList.
 * Allocates the memory,
 * sets head and tail to null,
 * and sets count to 0.
 * 
 * @authors Brendan Michael
 */
void createmList(){
    MCBList = (mList*)kmalloc(sizeof(mList));
    MCBList->head = NULL;
    MCBList->tail = NULL;
    MCBList->count = 0;
}
/**
 * Function: initializeHeap
 * -----------------------------
 * Allocates the heap memory specified 
 * by the parameter size using kmalloc.
 * 
 * @param size- The size of the heap in bytes. 
 * @authors Bryce Williams
 */
int initializeHeap(int size){
    
    heap_top = (void *)kmalloc(size + sizeof(CMCB_t)); // top of heap block
    if(heap_top == NULL){
        printlnMessage("failed to allocate memory for heap");
        return -1;
    }
    //memory is allocated for heap if reach here
    CMCB_t* heap_start = (CMCB_t*) heap_top; 
    heap_start->type = FREE_MEMORY;
    heap_start->size = size;
    heap_start->start = (u32int)(heap_top + sizeof(CMCB_t)); //how we get address of pointer
    heap_start->prev = NULL;
    heap_start->next = NULL;
    strcpy(heap_start->namePCB,"Initial");
    mListInsert(MCBList,heap_start); //insert block into MCB list
    return size;
}

/**
 * Function: allocateMemory
 * -----------------------
 * Allocates memory from the heap by 
 * taking in a specific amount of bytes.
 * Uses the first fit method for the memory 
 * blocks
 * @authors Brenden Michael, Farhan Shahbaz
 */

u32int allocateMemory(u32int bytes){
    CMCB_t *current = MCBList->head;
    //required size for mem and MCB
    int requiredSize = bytes + sizeof(CMCB_t);
    while(current != NULL && ( current->size <= requiredSize || current->type == ALLOCATED_MEMORY) ){
        current = current->next;
    }

    //No Free Memory of Required Size
    if(current == NULL){
        klogv("not enough memory");
        return NULL;
    }
    mListRemove(MCBList, current); 
    current->next = NULL;
    current->prev = NULL;
    //Get Address of selected Free Block
    u32int currAddress = current->start;
    //Create new Free CMCB at the end of the block that was just allocated
    CMCB_t* new = (CMCB_t*) (currAddress+bytes); 
    new->type = FREE_MEMORY; 
    new->next = NULL;
    new->prev = NULL;
    //  Remaining Free  requested space for new CMCB
    new->size = current->size - bytes - sizeof(CMCB_t);
    new->start = (u32int) (currAddress+requiredSize);
    strcpy(new->namePCB,"newBlock");
    mListInsert(MCBList,new);  
    current->type = ALLOCATED_MEMORY; 
    current->size = bytes;
    mListInsert(MCBList,current);//current is allocated memory block
    return (u32int) new->start;
}

/**
 * Function: freeMem
 * -----------------------------
 * Frees up memory specified by the 
 * address of ptr. Returns 0 if the 
 * address does not exist or the block
 * is already free.Returns 1 if 
 * succesfully freed. 
 * 
 * @param ptr- Pointer to memory location that needs to be freed.  
 * @authors Bryce Williams
 */
int freeMem(void *ptr){
    CMCB_t* current = MCBList->head;
    while(current != NULL){ 
        if(current->start == (u32int) ptr){
            break;
        }
        current = current->next;
    }
     //Couldn't find block
    if(current == NULL){
        return 0;
    }
     // memory is already free
    if(current->type == FREE_MEMORY){
        return 0;
    }

    merge(current); 
    
    return 1;
}

/**
 * Function: merge
 * -----------------------------
 * Merges the memory specified by
 * current with the rest of the 
 * free memory if possible. 
 * 
 * @param current- pointer to the memory address that needs to be merged.  
 * @authors Brendan Michael 
 */
void merge(CMCB_t *current){
    if(current->next->type == FREE_MEMORY && current->prev->type == FREE_MEMORY){
        int totalSize = current->size + current->next->size + current->prev->size + 2 * sizeof(CMCB_t); // plus size of two
        u32int newStart = current->prev->start;
        mListRemove(MCBList,current->prev);
        mListRemove(MCBList,current->next);
        mListRemove(MCBList,current);
        CMCB_t* newFree = (CMCB_t*) (newStart); 
        newFree->type = FREE_MEMORY;
        newFree->size = totalSize;
        newFree->start = newStart;
        strcpy(newFree->namePCB,"newBlock");
        mListInsert(MCBList,newFree);
    }
    else if(current->next->type == FREE_MEMORY){
        int totalSize = current->size + current->next->size + sizeof(CMCB_t); //plus size of one
        u32int newStart = current->start;
        mListRemove(MCBList,current->next);
        mListRemove(MCBList,current);
        CMCB_t* newFree = (CMCB_t*) (newStart); 
        newFree->type = FREE_MEMORY;
        newFree->size = totalSize;
        newFree->start = newStart;
        strcpy(newFree->namePCB,"newBlock");
        mListInsert(MCBList,newFree);
    }
    else if(current->prev->type == FREE_MEMORY){
        int totalSize = current->size + current->prev->size + sizeof(CMCB_t); //plus size of one
        u32int newStart = current->prev->start;
        mListRemove(MCBList,current->prev);
        mListRemove(MCBList,current);
        CMCB_t* newFree = (CMCB_t*) (newStart); 
        newFree->type = FREE_MEMORY;
        newFree->size = totalSize;
        newFree->start = newStart;
        strcpy(newFree->namePCB,"newBlock");
        mListInsert(MCBList,newFree);
    }
    else{
        current->type = FREE_MEMORY;
    }
}

/**
 * Function: heapIsEmpty
 * -----------------------
 * Checks to see if the heap is
 * empty. Returns 0 for false and
 * 1 for true
 * @authors Farhan Shahbaz
 */
//checks to see if heap iis empty
int heapIsEmpty(){
   CMCB_t* current = MCBList->head;
   while(current != NULL){
    if(current->type == ALLOCATED_MEMORY){
        return 0;
    }
    current = current->next;
}
return 1;
}

/**
 * Function: printCMCB
 * -----------------------
 * Prints the block information and block address.
 * Takes the pointer to the memory block.
 * 
 * @authors Selim Demircan
 */
void printCMCB(CMCB_t* block){
    printlnMessage("==============");
    //prints block type
    if(block->type == FREE_MEMORY){
        printlnMessage("Free block");
    }
    //prints block type
    else if(block->type == ALLOCATED_MEMORY){
        printlnMessage("Allocated block");
    }
    else{
        printlnMessage("Error, type not set");
    }
    char address[10];
    printMessage("Start of block:");
    itoa(block->start, address,10);
    printlnMessage(address);
    char size[10];
    printMessage("Size of block:");
    itoa(block->size, size,10);
    printlnMessage(size);
    printMessage("Name:");
    printlnMessage(block->namePCB);
    if(block->prev != NULL){
        printMessage("prev is: ");
        printlnMessage(block->prev->namePCB);
    }else{
        printlnMessage("prev is null");
    }
    if(block->next != NULL){
        printMessage("next is: ");
        printlnMessage(block->next->namePCB);
    }
    else{
        printlnMessage("next is null");
    }
    printlnMessage("==============");
}

/**
 * Function: printAllocated
 * -----------------------
 * Prints all allocated blocks of memory.
 * 
 * @authors Selim Demircan
 */
void printAllocated(){
    CMCB_t* current = MCBList->head;
    printlnMessage("MCB LIST");
    while(current != NULL){
        if(current->type == ALLOCATED_MEMORY){
        printCMCB(current);
    }
        current = current->next;
    }
    printMessage("\n");
    
}

/**
 * Function: printFree
 * -----------------------
 * Prints all free blocks of memory.
 * 
 * @authors Selim Demircan
 */
void printFree(){
    CMCB_t* current = MCBList->head;
    printlnMessage("MCB LIST");
    while(current != NULL){
        if(current->type == FREE_MEMORY){
        printCMCB(current);
    }
        current = current->next;
    }
    printMessage("\n");
    
}
/**
 * Function: printAll
 * -----------------------
 * Prints allocated and free
 * blocks of memory.
 * 
 * @authors Selim Demircan
 */
void printAll(){
    CMCB_t* current = MCBList->head;
    printlnMessage("MCB LIST");
    while(current != NULL){
        printCMCB(current);
        current = current->next;
    }
    printMessage("\n");
    
}
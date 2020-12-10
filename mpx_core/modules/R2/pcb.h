#ifndef _pcb_H
#define _pcb_H
#define READY 0
#define RUNNING 1 
#define BLOCKED 2 
#define SUSPENDED 1 
#define NOTSUSPENDED 0
#define SYSTEM 0
#define APPLICATION 1
#define stackSize 1024

//Using doubly-linked list structure (next/prev)
typedef struct PCB
{
	char name[9];
	int class;
	int priority;
	int status;
	int suspended;
	struct PCB *next;
	struct PCB *prev;
	unsigned char stack[1024];
	unsigned char *stackTop;
	unsigned char *stackBase;
}PCB;


/**
 * Function: allocatePCB
 * ---------------------
 * Dynamically allocates a PCB.
 *
 * @return the allocated PCB
 * @author Brendan Michael
 */
PCB *allocatePCB();

/**
 * Function: freePCB
 * ---------------------
 * Frees memory associated
 * with a PCB
 *
 * @return 1 if no error
 * @author Brendan Michael
 */
int freePCB(PCB *p);

/**
 * Function: setupPCB
 * ---------------------
 * Sets a PCBS fields
 * according to the provided
 * parameters 
 *
 * @param *name: the name for the PCB
 * @param class: int class value
 * to be assigned
 * @param priority: int priority value
 * to be assigned
 * @return a pointer to the 
 * set up PCB
 * @author Brendan Michael
 */
PCB *setupPCB(char *name, int class, int priority);
/**
 * Function: printPCB
 * ---------------------------------------
 * Displays the name, status, and class
 * of the selected PCB.
 * @param pcb_p pointer of a declared PCB.
 * @author Bryce Williams
 */
void printPCB(PCB* pcb_p);
#endif

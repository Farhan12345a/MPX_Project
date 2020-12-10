#ifndef _processManagment_H
#define _processManagment_H
/**
 * Function: initQueues
 * ---------------------
 * Initializes the ready 
 * and blocked queues
 *
 * @author Brendan Michael
 */
void initQueues();

/**
 * Function: createPCB
 * ---------------------
 * Allocates memory for a
 * new PCB
 *
 * @author Brendan Michael
 */
void createPCB(char *name,int class, int priority);

/**
 * Function: showReady
 * ----------------------------------------------------------------------------
 * Displays the pcb's currently in the ready state.
 * Checks to see if readyQueue is empty.
 * If it is empty displays an error message.
 * @author Bryce Williams
 */
int showReady();

/**
 * Function: showBlocked
 * ---------------------
 * Displays the blocked queue
 *
 * @return 1 if no error
 * @author Brendan Michael
 */
#include "pcb.h"
 /**
  * Function: createPCB
  *                                      
  * Creates PCB  with user specifies
  * name and priority. If user enters 
  * a name already in use, display error
  * message.
  * @param name user entered name of PCB
  * @param class int value representing class of PCB
  * @param priority int value representing priority of PCB
  * @author Bryce Williams
*/
struct Queue* createQueue(char* name);
void showAll();
int deletePCB(char *name);
/**

* Function: showBlocked

* ----------------------------------------------------------------------------

* Displays queues that are currently in the blocked state.

* If nothing is in block queue displays message that

* the queue is currently empty. 

* @author Bryce Williams

 */
void insertPCB(PCB *pcb);
PCB *getNextReady();
void showBlocked();
int blockPCB(char *name);
int unblockPCB(char *name);
/**
 * Function: showPCB
 * ----------------------------------------------------------------------------
 * Finds pcb based on user entered name.
 * Searches in readyQueue then blockedQueue.
 * @param name name of the PCB to search for.
 * @author Bryce Williams
 */
void showPCB(char* name);
void setPCBpriority(char *name, int priority);
void noQueueError();
int deletePCB(char *name);
void removeReady(PCB *p);
void removeBlocked(PCB *p);
int resumePCB(char *name);
void setPCBpriority(char *name, int priority);
int suspendPCB(char *name);
int clearQueues();
#endif
/**
 * Function: resumePCB
 * -------------------------------------------------
 * resume searches for process in blocked queue
 * If we do not find it return null.
 * Then search for it in ready queue.
 * @param name name of the PCB to search for.
 * @author Bryce Williams
 */
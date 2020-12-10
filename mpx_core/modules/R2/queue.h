#ifndef _queue_H
#define _queue_H
typedef struct Queue
{
      int count;
      struct PCB *head;
      struct PCB *tail;
      char * name;

} Queue;


/**
 * Function: createQueue
 * ---------------------
 * Creates and initializes an
 * empty queue. 
 * 
 * @return the created queue
 * @author Brendan Michael
 */


/**
 * Function: findPCB()
 * ---------------------
 * Finds and returns a PCB 
 * based on the given name and
 * queue. 
 *
 * @param *name: the PCB name
 * @param *q: the queue to 
 * look through
 * @return the found PCB
 * @author Brendan Michael
 */

/**
 * Function: printQueue
 * ---------------------
 * Prints out the contents of 
 * the given queue
 *
 * @param *q: the queue to 
 * print out 
 * @author Brendan Michael
 */
void printQueue(Queue *q);

/**
 * Function: findPCB()
 * ---------------------
 * Finds and returns a PCB 
 * based on the given name and
 * queue. 
 *
 * @param *name: the PCB name
 * @param *q: the queue to 
 * look through
 * @return the found PCB
 * @author Brendan Michael
 */
struct PCB *findPCB(char *name, Queue *q);

/**
 * Function: queueInsert
 * ---------------------
 *  
 * @param *q: the queue to be
 * inserted into
 * @param *pcb: the pcb to be
 * inserted 
 * @return 1 if no error occured
 * @author Bryce Williams and Brendan Michael
 */
int queueInsert(Queue *q, struct PCB *pcb);
int queueRemove(Queue *q, struct PCB *pcb);
int isEmpty(Queue *q);
void peek(Queue *q);
#endif
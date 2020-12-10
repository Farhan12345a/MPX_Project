#include <string.h>
#include <system.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "queue.h"
#include "pcb.h"
#include "processManagement.h"
#include "../R1/comhand.h"

//Not that complicated only chose to use 2 Queues, more complicated need more queues are seperate structures
//Ex: nonsuspended ready/suspended ready
Queue *readyQueue;
Queue *blockedQueue;
int queuesCleared; //If it is empty set to 1, if not it is 0.

Queue* createQueue(char* name){
	Queue *q = sys_alloc_mem(sizeof(Queue));
	q->head = NULL;
	q->tail = NULL;
	q->count = 0;
	q->name = name;
	return q;
}

PCB *getNextReady(){
	if(queuesCleared){ //for shutdown
		return NULL; //if queues are cleared just return null
	}
	if(!isEmpty(readyQueue)){ 
		PCB* pcb = readyQueue->head;
		//searching for the next ready PCB that is not suspended. If it is suspended keep on going.
		while(pcb != NULL && pcb->suspended == SUSPENDED){
			pcb = pcb->next;
		}
		if(pcb != NULL){
			queueRemove(readyQueue,pcb); //There is a PCB to remove, remove it
		}
		return pcb;
	}
	else{
		return NULL;
	}
}

void noQueueError(){
	printRed();
	printMessage("\nQueues are empty: cannot perform command");
	printGreen();
}

int clearQueues(){
	queuesCleared = 1;
	return queuesCleared;
}


void initQueues(){
	readyQueue = createQueue("ready");
	blockedQueue = createQueue("blocked");
}

int deletePCB(char *name){
	PCB *delete = findPCB(name,readyQueue);
	if(delete != NULL){
		//if the name is the infinite process and if its not suspended then we cannot delete it. Only can be deleted if suspended
		if(strcasecmp(name, "INF Proc") == 0 && delete->suspended != SUSPENDED){
			printMessage("Cannot delete infinite process if not suspended");
		}
		removeReady(delete);
		freePCB(delete);
		return 1;
	}
	if(delete == NULL){
		delete = findPCB(name,blockedQueue);
		if(delete != NULL){
			removeBlocked(delete);
			freePCB(delete);
			return 1;
		}
	}
	printRed();
	char *notFound = "\nError: Failure to delete PCB, PCB not found in any queue.";
	int notFoundSize = 59;
	sys_req(WRITE, DEFAULT_DEVICE, notFound, &notFoundSize);
	printGreen();
	return 0;
}

void insertPCB(PCB *pcb){
	if(pcb->status == BLOCKED){
		queueInsert(blockedQueue,pcb);
	} else if (pcb->status == READY){
		queueInsert(readyQueue,pcb);
	}
}

void createPCB(char *name,int class, int priority){
	if(findPCB(name,readyQueue) != NULL){
		printRed();
		printlnMessage("\nProcess with that name already exist. Please try another.");
		printGreen();
		return;
	}
	if(priority < 0 || priority > 9){
		printRed();
		printlnMessage("\nInvalid priority entry.Please enter value between 0-9.");
		printlnMessage("Please try the 'create' function again using valid priority.");
		printGreen();
		return;
	}
	if(class != 'A' && class != 'S'){
		printRed();
		printlnMessage("\nInvalid class entry.Please enter values 'S' or 'A'.");
		printlnMessage("Please try the 'create' function again using a valid class.");
		printGreen();
		return;
	}
	PCB *p = NULL;
	if(class == 'A'){
		 p = setupPCB(name,APPLICATION,priority);
		//queueInsert(readyQueue,p); //comment out and it works
	}else{
		p = setupPCB(name,SYSTEM,priority);
		//queueInsert(readyQueue,p); //comment out and it works
	}
	if(p == NULL){
		class = 0;
	}
}
//shows all in suspended ready and ready queue
int showReady(){
	if(readyQueue->count != 0){
		printQueue(readyQueue);
		return 1;
	}
	else{
		char *readyError = "Ready Queue is empty:";
		int errorSize = 20;
		printRed();
		sys_req(WRITE, DEFAULT_DEVICE, readyError, &errorSize);
		printGreen();
	}
	return 0;
}
//Sets prio and reinsters process into the correct queue
void setPCBpriority(char *name, int priority){
	PCB *specificPCB = findPCB(name, readyQueue);
	if(specificPCB == NULL){
		findPCB(name, blockedQueue);
	}
	//Error check if PCB doesn't exist ****
	if(specificPCB == NULL){
		printlnMessage("\nError: PCB doesn't exist");
		return;
	}
	//error check to see between 0 9
	if(priority < 0 || priority > 9){
		printRed();
		printlnMessage("Error: PCB priority is not in range [0-9]");
		printGreen();
		return;
	}else{
		specificPCB->priority = priority; 
	}

	deletePCB(name);
	//Reinserts PCB in the queue
	if(specificPCB->status == READY){
		queueInsert(readyQueue, specificPCB);
	}else{
		queueInsert(blockedQueue, specificPCB);
	}
}

void showAll(){
		showReady();
		showBlocked();
}


void showBlocked(){
	if(blockedQueue->count != 0){
		printQueue(blockedQueue);
	}
	else{
		char *blockedError = "Blocked Queue is empty";
		int errorSize = 20;
		printRed();
		sys_req(WRITE, DEFAULT_DEVICE, blockedError, &errorSize);
		printGreen();
	}
}

void removeReady(PCB *p){
	queueRemove(readyQueue,p);
}

void removeBlocked(PCB *p){
	queueRemove(blockedQueue,p);
}

int blockPCB(char *name){
		PCB *blockee = findPCB(name, readyQueue);
		if(blockee == NULL){
			printRed();
			char *notFound = "\nError: Failure to block PCB, PCB not found in ready queue.";
			int notFoundSize = 59;
			sys_req(WRITE, DEFAULT_DEVICE, notFound, &notFoundSize);
			printGreen();
			return 0;
		}
		else{
			removeReady(blockee);
			blockee->status = BLOCKED;
			queueInsert(blockedQueue, blockee);
			return 1;
		}

	printRed();
	printMessage("\nQueue is empty: no PCB to be blocked");
	printGreen();
	return 0;
}

int unblockPCB(char *name){

		PCB *unblockee = findPCB(name, blockedQueue);
		if(unblockee == NULL){
			printRed();
			char *notFound = "\nError: Failure to unblock PCB, PCB not found in blocked queue.";
			int notFoundSize = 63;
			sys_req(WRITE, DEFAULT_DEVICE, notFound, &notFoundSize);
			printGreen();
			return 0;
		}
		else{
			queueRemove(blockedQueue, unblockee);
			unblockee->status = READY;
			queueInsert(readyQueue, unblockee);
			return 1;
		}
	printRed();
	printMessage("\nQueue is empty: no PCB to be unblocked");
	printGreen();
	return 0;
}
//Places PCB in the not suspended state and reinsters into queue
int resumePCB(char *name){
	PCB *resume = findPCB(name, blockedQueue);
	if(resume == NULL){
		resume = findPCB(name,readyQueue);
	}
	if(resume == NULL){
		printRed();
		char *notFound = "\nError: Failure to resume PCB, PCB not found.";
		int notFoundSize = 63;
		sys_req(WRITE, DEFAULT_DEVICE, notFound, &notFoundSize);
		printGreen();
		return 0;
	}
	else{
		resume->status = READY;
		resume->suspended = NOTSUSPENDED; 
	}

	return 0;
}
//Places PCB into suspended state and reinsterts into queue
//error checks for valid PCB name
int suspendPCB(char *name){
	PCB *suspended = findPCB(name, blockedQueue);
	if(suspended ==NULL){
		suspended =findPCB(name,readyQueue); 
	}
	if(suspended == NULL){
		printRed();
		char *notFound = "\nError: Failure to suspend PCB, PCB not found.";
		int notFoundSize = 63;
		sys_req(WRITE, DEFAULT_DEVICE, notFound, &notFoundSize);
		printGreen();
		return 0;
	}
	else{
		suspended->suspended = SUSPENDED; //lol
	}

	return 0;
}

void showPCB(char* name){
	PCB* pcb_p = findPCB(name,readyQueue);
	if(pcb_p == NULL){
		pcb_p = findPCB(name,blockedQueue);
		printMessage("\nIn blocked Queue\n");
		printPCB(pcb_p);
		return;
	}
	if(pcb_p == NULL){
		printRed();
		printMessage("Error: Failure: PCB not found.");
		printGreen();
		return;
	}
	printMessage("\nIn ready Queue\n");
	printPCB(pcb_p);
}



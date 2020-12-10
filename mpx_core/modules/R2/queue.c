#include "queue.h"
#include <string.h>
#include <system.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "pcb.h"

char* readyTitle = "READY QUEUE\n";
char* blockedTitle = "BLOCKED QUEUE\n";
int titleSize = 20;
char* border = "==============\n";
int borderSize = 16;

int isEmpty(Queue *q){
	return q->count == 0;
}

int queueInsert(Queue *q, PCB *pcb){ 
	if(q->head == NULL){
		q->tail = pcb;
		q->head = q->tail;
		q->count++;
		return 1;
	}
	
	else if(pcb->status == BLOCKED){
		PCB *current = q->head;
		PCB *prev = NULL;
		while(current != NULL){
			prev = current;
			current = current->next;
		}
		//inserting at the head
		if(prev == NULL){
			pcb->next = current;
			current->prev = pcb;
			q->head = pcb;
		}
		//not inserting at the head
		else{
			pcb->next = current;
			pcb->prev = prev;
			prev->next = pcb;
			if(current != NULL){
				current->prev = pcb;
			}
			//inserting at the tail
			else{
				q->tail = pcb;

			}
		}
	}
	//inseting into ready
	else{
		PCB *current = q->head;
		PCB *prev = NULL;
		//If not blocked insert into correct prio
		while(current != NULL && current->priority >= pcb->priority){
			prev = current;
			current = current->next;
		}
		//inserting at the head
		if(prev == NULL){
			pcb->next = current;
			current->prev = pcb;
			q->head = pcb;
		}
		//not inserting at the head
		else{
			pcb->next = current;
			pcb->prev = prev;
			prev->next = pcb;
			if(current != NULL){
				current->prev = pcb;
			}
			//inserting at the tail
			else{
				q->tail = pcb;

			}
		}
	}
	q->count++;
	return q->count;
}

int queueRemove(Queue *q, PCB *pcb){
	PCB *current = q->head;
	PCB *prev = NULL;
	while(current != NULL &&  strcmp(current->name , pcb->name)!= 0){
		prev = current;
		current = current->next;
	}

		//remove at the head
	if(prev == NULL){
		q->head = pcb->next;
		pcb->next = NULL;
	}
		//not removing at head
	else{
		if(current->next != NULL){
			prev->next = current->next;
			current->next->prev = prev;
		}
		else{//removing at tail
			current->prev = q->tail;
			prev->next = NULL;
		}
		//disconnects current from list
		current->next = NULL;
		current->prev = NULL;
	}
	q->count--;
	return 1;

}



void peek(Queue *q){
	char *name = q->head->name;
	int nameSize = 10;
	sys_req(WRITE, DEFAULT_DEVICE, name, &nameSize);
}


PCB *findPCB(char *name,Queue *q){
	if(q->head == NULL){
		return NULL;
	}
	PCB *current = q->head;
	while(current != NULL){
		if(strcmp(current->name,name) == 0){
			return current;
		}
		else{
			current = current->next;
		}
	}
	return NULL;
}

void printQueue(Queue *q){
	PCB *current = q->head;
	int nameSize = 100;
	sys_req(WRITE, DEFAULT_DEVICE, "\n", &nameSize);
	if(strcmp(q->name,"ready") == 0){
		while(current != NULL){
			sys_req(WRITE, DEFAULT_DEVICE, readyTitle, &titleSize);
			sys_req(WRITE, DEFAULT_DEVICE, border, &borderSize);
			printPCB(current);
			current= current->next;
		}
	}
	else{
		while(current != NULL){
		sys_req(WRITE, DEFAULT_DEVICE, blockedTitle, &titleSize);
		sys_req(WRITE, DEFAULT_DEVICE, border, &borderSize);
		printPCB(current);
		current= current->next;
	}
	}
	
}


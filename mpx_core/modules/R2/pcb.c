#include <string.h>
#include <system.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "queue.h"
#include "pcb.h"
#include "../R3/context.h"

//returns pointer to block that was allocated
PCB *allocatePCB(){
	PCB *p = sys_alloc_mem(sizeof(PCB)); //allocate enough memory for data scructure of type pcb
	p->class = SYSTEM;
	p->priority = 0;
	memset(p->stack,'\0',1024);
	p->stackBase = p->stack; 
	p->stackTop = p->stackBase +1024 - sizeof(Context);
	return p;
}

int freePCB(PCB *p){
	return sys_free_mem(p);
}

PCB *setupPCB(char *name, int class, int priority){
	PCB *p = allocatePCB();
	strcpy(p->name,name);
	p->class = class;
	p->priority = priority;
	p->status = READY;
	p->suspended = NOTSUSPENDED;
	return p;
}

void printPCB(PCB* pcb_p){
	printMessage("name: ");
	printlnMessage(pcb_p->name);

	printMessage("status: ");
	if(pcb_p->status == READY){
		printlnMessage("Ready");
	}
	else if(pcb_p->status == RUNNING){
		printlnMessage("Running...");
	}
	else if(pcb_p->status == BLOCKED){
		printlnMessage("Blocked");
	}
	
	printMessage("suspended: ");
	if(pcb_p->suspended == SUSPENDED){
		printlnMessage("Suspended");
	}else{
		printlnMessage("Not Suspended");
	}

	printMessage("class: ");
	if(pcb_p->class == SYSTEM){
		printlnMessage("SYSTEM");
	}
	if(pcb_p->class == APPLICATION){
		printlnMessage("APPLICATION");
	}

	printMessage("priority: ");
	char message[20];
	printlnMessage(itoa(pcb_p->priority,message,10));
}

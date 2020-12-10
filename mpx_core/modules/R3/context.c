#include "../R2/pcb.h"
#include <system.h>
#include "context.h"
#include "../mpx_supt.h"
#include "../R2/processManagement.h"
#include "test.h"
#include "string.h"
#include "../R6/IOqueue.h"
#include "../R6/driver.h"
#include <core/io.h>
#include "../R2/pcb.h"
extern dcb_t* DCB;
 

PCB* cop; 
Context* old;  
extern param params; 
typedef void(*func_ptr) ();


extern ioqueue_t ioqueue;

/**
* Function: loadProcess
* ------------------------------- 
* Loads process into PCB. 
*
* @author Farhan Shahbaz
*/
void loadProcess(char *name, func_ptr func){
	PCB * new_pcb = setupPCB(name , 1 , 1); 
	//Now we set up context of PCB
	Context * cp = (Context *)(new_pcb -> stackTop); 
	memset (cp, 0, sizeof(Context)); 
	cp -> fs = 0x10;
	cp -> gs = 0x10;
	cp -> ds = 0x10;
	cp -> es = 0x10;
	cp -> cs = 0x8;
	cp -> ebp = (u32int)(new_pcb -> stackBase);
	cp -> esp = (u32int)(new_pcb -> stackTop); 
	cp -> eip = (u32int) func ;
	cp -> eflags = 0x202;
	new_pcb->suspended = SUSPENDED;
	insertPCB(new_pcb);
}

void loadr3(){
	loadProcess("process1", proc1);
	loadProcess("process2", proc2);
	loadProcess("process3", proc3);
	loadProcess("process4", proc4);
	loadProcess("process5", proc5);
	printMessage("+++++++++");
	printMessage("\nR3 LOADED");
	printMessage("\n+++++++++");
}
u32int* sys_call(Context *registers){
	//klogv("SYSCALL\n");
	PCB* ready = NULL;
	ready = getNextReady();
	//klogv("SYSCALL GETNEXTREADY\n");
	//If there is no currently operating process, make old store registers of current process
	if(cop == NULL){
		old = registers;
	}
	else{
		//if process calls sysreq and reqests idle 
		if(params.op_code == IDLE){
			//klogv("SYSCALL IDLE\n");
			cop->stackTop = (unsigned char*) registers; 
			cop->status = READY;
			//showReady();
			insertPCB(cop);
			//klogv("SYSCALL IDLE DONE\n");
		}
		if(params.op_code == EXIT){
			freePCB(cop);
		}
		if(is_io_module_active() && (params.op_code == READ || params.op_code == WRITE)){
			//klogv("READ/WRITE OPPERATION REQUESTED\n");
			iod_t* iod = (iod_t*)sys_alloc_mem(sizeof(iod_t));
			iod->op_code = params.op_code;
			iod->buffer_ptr = params.buffer_ptr;
			iod->count_ptr = params.count_ptr;
			iod->next = NULL;
			iod->pcb = cop;
			cop->stackTop = (unsigned char*) registers;
			cop->status = BLOCKED;
			insertPCB(cop);
			enqueue(&ioqueue,iod);
			//io_scheduler(); //call io scheduler


		}

	}
	//if ready process is found 
	if(ready != NULL){
		cop = ready; //process from ready queue
		cop->status = RUNNING; //set to running
		//klogv(cop->name);
		//klogv("SYSCALL RETURNING STACKTOP\n");
		return (u32int*) cop->stackTop; 
	}
	else{ // no ready process
		//klogv(cop->name);
		//klogv("SYSCALL RETURNING OLD\n");
		return (u32int*) old; //return to old process that called
	}
}

void yield(){
	asm volatile ("int $60"); //generates interrupt 60 
}

void io_scheduler() {
iod_t* iod = NULL;
	if(!isEmpty_IO(&ioqueue)){
		if(DCB->status == STATUS_IDLE){
			 iod = peek_IO(&ioqueue);	//checks the request
			if(iod->op_code == READ){
				if(-1 == com_read(iod->buffer_ptr, iod->count_ptr)){
					klogv("Problem with reading");
				}
			}else if(iod->op_code == WRITE){
				klogv(iod->pcb->name);
				klogv("Calling comwrite");
				com_write(iod->buffer_ptr, iod->count_ptr);
					
			}
		}

	}

// If completed,
	if(*(DCB->e_flag) == 1){ //event has completed
		iod = dequeue(&ioqueue); //remove from queue
		// unblock the corresponding PCB and remove it from queue
		unblockPCB(iod->pcb->name);
		*(DCB->e_flag) = 0;
		DCB->buffer_ptr = NULL;
		DCB->count_ptr =NULL;
		// call com_read() or com_write() on the next iod depending on the op code.
		//io_scheduler(); //recursive call to io_scheduler
	}
}

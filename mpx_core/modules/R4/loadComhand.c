#include "../R2/pcb.h"
#include <system.h>
#include "../R3/context.h"
#include "../mpx_supt.h"
#include "../R2/processManagement.h"
#include "string.h"
#include "../R1/comhand.h"
#include "alarm.h"
/**
* Function: loadComhand
* ----------------------------------
* Loads comhand as a system
* process with a high set priority.
*
* @author Bryce Williams
*/
void loadComhand(){
	PCB * new_pcb = setupPCB("commandHandler" , 0 , 9); 
	Context * cp = (Context *)(new_pcb -> stackTop); 
	memset (cp, 0, sizeof(Context)); 
	cp -> fs = 0x10;
	cp -> gs = 0x10;
	cp -> ds = 0x10;
	cp -> es = 0x10;
	cp -> cs = 0x8;
	cp -> ebp = (u32int)(new_pcb -> stackBase);
	cp -> esp = (u32int)(new_pcb -> stackTop); 
	cp -> eip = (u32int) comhand ;// How we turned comhand into process... The function comhand adress is stored in instruction pointer register. 
	cp -> eflags = 0x202;
	new_pcb->suspended = NOTSUSPENDED; 
	insertPCB(new_pcb);
}
/**
* Function: loadIdle
* --------------------------
* Loads idle as a system
* process with a low set
* piority
*
* @author Bryce Williams
*/
//same that is happening above. but for idle isntead ofcomhand. Taking function idle and setting it to instruction point register to make idle a command.
void loadIdle(){
	PCB * new_pcb = setupPCB("IDLE" , 0 , 1); 
	Context * cp = (Context *)(new_pcb -> stackTop); 
	memset (cp, 0, sizeof(Context)); 
	cp -> fs = 0x10;
	cp -> gs = 0x10;
	cp -> ds = 0x10;
	cp -> es = 0x10;
	cp -> cs = 0x8;
	cp -> ebp = (u32int)(new_pcb -> stackBase);
	cp -> esp = (u32int)(new_pcb -> stackTop); 
	cp -> eip = (u32int) idle ;// The function correlating to the process , ie. Proc1cp - > eflags = 0 x202 ;return new_pcb ;
	cp -> eflags = 0x202;
	new_pcb->suspended = NOTSUSPENDED;
	insertPCB(new_pcb);
}
/**
* Function: loadTimeDaemon
* -----------------------------------
* Loads TimeDaemon as a system
* process with a high set priority.
*
* @author Bryce Williams
*/
void loadTimeDaemon(){
	PCB * new_pcb = setupPCB("TimeDaemon" , 0 , 1); 
	Context * cp = (Context *)(new_pcb -> stackTop); 
	memset (cp, 0, sizeof(Context)); 
	cp -> fs = 0x10;
	cp -> gs = 0x10;
	cp -> ds = 0x10;
	cp -> es = 0x10;
	cp -> cs = 0x8;
	cp -> ebp = (u32int)(new_pcb -> stackBase);
	cp -> esp = (u32int)(new_pcb -> stackTop); 
	cp -> eip = (u32int) timeDaemon ;// The function correlating to the process , ie. Proc1cp - > eflags = 0 x202 ;return new_pcb ;
	cp -> eflags = 0x202;
	new_pcb->suspended = NOTSUSPENDED;
	insertPCB(new_pcb);
}
/**
* Function: loadInfiniteProcess
* -------------------------------------------
* Loads an infinite process as an application 
* with a low set priority.
*
* @author Brendan Michael
*/
void loadInfiniteProcess(){
	PCB * new_pcb = setupPCB("INF Proc" , 1 , 1); 
	Context * cp = (Context *)(new_pcb -> stackTop); 
	memset (cp, 0, sizeof(Context)); 
	cp -> fs = 0x10;
	cp -> gs = 0x10;
	cp -> ds = 0x10;
	cp -> es = 0x10;
	cp -> cs = 0x8;
	cp -> ebp = (u32int)(new_pcb -> stackBase);
	cp -> esp = (u32int)(new_pcb -> stackTop); 
	cp -> eip = (u32int) infiniteProcess ;// The function correlating to the process , ie. Proc1cp - > eflags = 0 x202 ;return new_pcb ;
	cp -> eflags = 0x202;
	new_pcb->suspended = NOTSUSPENDED;
	insertPCB(new_pcb);
}

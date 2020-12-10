#ifndef _context_H
#define _context_H
//Syscall gets from pushing these registers in rq.s
typedef struct Context {
	u32int gs, fs, es, ds;
	u32int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	u32int eip, cs, eflags;
}Context;



/**
 * Function: loadr3
 * ---------------------
 * Loads test processes into memory 
 * in a suspended ready state
 * @return the created queue
 * @author Brendan Michael, Selim Demircan 
 */
void loadr3();

/**
 * Function: sys_call
 * ---------------------
 * Performs context switching beteween
 * current process and next ready process
 * 
 * @return u32int*: Registers
 * @author Brendan Michael, Farhan Shahbaz
 */
u32int* sys_call(Context* registers);


/**
 * Function: yield
 * ---------------------
 * Causes the comhand to 
 * give up CPU time to other
 * processes. Processes in the 
 * ready queue will be executed
 *
 * @author Brendan Michael 
 */
void yield();

/*!
+* io_scheduler() creates an io device for the PCB requesting IO.
+* @param (the params you give it depends on the design of your system)
*/
void io_scheduler();

#endif
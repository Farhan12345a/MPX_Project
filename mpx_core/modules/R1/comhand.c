#include <string.h>
#include <system.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "comhand.h"
#define RED "\033[91m"
#define GREEN "\033[92m"
#include "miscCommands.h"
#include "date.h"
#include "time.h"
#include "../R4/alarm.h"
#include "../R4/loadComhand.h"
#include "../R2/R2Commands.h"
#include "../R2/processManagement.h"
#include "../R3/context.h"
#include "../R5/memoryCommands.h"
#include "../R5/memoryManagment.h"

#define WHITE "\033[97m"
#define UP "\033[A"
#define DOWN "\033[B"

//random commentss
int introSize = 32;
char intro[] = "\nType help for commands list";
int cmdBufferSize = 99;
char cmdBuffer[100];


/**
 * Function: comhand
 * -----------------------
 * Executes commands based on input gathered
 * from calling polling. Displays an error 
 * message if the entered command does not
 * exist.
 * 
 * @authors Selim Demircan, Brendan Michael
 * Bryce Williams, Farhan Shahbaz
 */

int comhand()
{
    int quit = 0;
    sys_req(WRITE, DEFAULT_DEVICE, intro, &introSize);
    while (!quit)
    {
        resetBuffer();
        poll();
        if (strcasecmp(cmdBuffer, "version") == 0)
        {
            version();
        }
        else if (strcasecmp(cmdBuffer, "help") == 0)
        {
            help();
        }
        else if (strcasecmp(cmdBuffer, "loadInfinite") == 0)
        {
            loadInfiniteProcess();    
        }

        else if (strcasecmp(cmdBuffer, "getTime") == 0)
        {
            getTimeCall();
        }
        else if (strcasecmp(cmdBuffer, "getDate") == 0)
        {
            getDate();
        }
        else if (strcmp(cmdBuffer, "set") == 0){
            setPriorityCall();
        }
        else if (strcmp(cmdBuffer, "setTime") == 0)
        { 
            setTimeCall();
        }
        else if (strcmp(cmdBuffer, "setDate") == 0)
        { 
            setDateCall();
        }
        else if (strcmp(cmdBuffer, "setDate") == 0)
        {

            setDateCall();
        }
        else if (strcasecmp(cmdBuffer, "showall") == 0)
        {
            showAll();
        }

        else if (strcasecmp(cmdBuffer, "yield") == 0)
        {
            yield();
        }

        else if (strcasecmp(cmdBuffer, "loadr3") == 0)
        {
            loadr3();
        }

        else if (strcasecmp(cmdBuffer, "showReady") == 0)
        {

            showReadyCall();
        }

        else if (strcasecmp(cmdBuffer, "showBlocked") == 0)
        {
            showBlocked();
        }

        else if (strcasecmp(cmdBuffer, "clear") == 0)
        {
            clear();
        }
        else if(strcasecmp(cmdBuffer, "suspend") == 0){
            suspendPCBCall();
        }
         else if(strcasecmp(cmdBuffer, "resume") == 0){
            resumePCBCall();
        }
        else if (strcasecmp(cmdBuffer, "deletePCB") == 0)
        {

            deletePCBCall();
        }
        else if (strcasecmp(cmdBuffer, "createAlarm") == 0)
        {
            createAlarmCall();
        }

        else if (strcasecmp(cmdBuffer, "showPCB") == 0)
        {
            showPCBCall();
        }
        else if(strcasecmp(cmdBuffer, "allocate") == 0){
            allocateMemoryCall();
        }else if(strcasecmp(cmdBuffer, "free") == 0){
            FreeMemoryCall();
        }else if(strcasecmp(cmdBuffer, "showfree") == 0){
            printFree();
        }
        else if(strcasecmp(cmdBuffer, "showallocated") == 0){
            printAllocated();
        }
         else if(strcasecmp(cmdBuffer, "printallMem") == 0){
            printAll();
        }
        else if(strcasecmp(cmdBuffer, "isEmpty") == 0){
            isEmptyCall();
        }
        else if (strcasecmp(cmdBuffer, "shutdown") == 0)
        {
            if (shutdown() == 1)
            {
                clearQueues();
                sys_req(EXIT, NULL, NULL, NULL);
                quit = 1;
            }

        }
        else
        {
            noSuchCommand();

        }
        sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
    }
    return 1;
}


void noSuchCommand()
{
    printRed();
    char *noCommand = "Command not found: type help for commands list\n";
    int noCommandSize = sizeof(noCommand) / sizeof(noCommand[0]);
    sys_req(WRITE, DEFAULT_DEVICE, noCommand, &noCommandSize);
    printGreen();
}

//reads command
void poll()
{
    sys_req(WRITE, DEFAULT_DEVICE, "\n", &introSize);
    sys_req(READ, DEFAULT_DEVICE, cmdBuffer, &cmdBufferSize);
    sys_req(WRITE, DEFAULT_DEVICE, "\n", &introSize);
}

void resetBuffer()
{
    memset(cmdBuffer, '\0', 100);
}

void printRed()
{
    sys_req(WRITE, DEFAULT_DEVICE, RED, &cmdBufferSize);
}

void printGreen()
{
    sys_req(WRITE, DEFAULT_DEVICE, GREEN, &cmdBufferSize);
}
void printWhite(){
    sys_req(WRITE, DEFAULT_DEVICE, WHITE, &cmdBufferSize);

}

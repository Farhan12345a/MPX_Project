#include <string.h>
#include <system.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "comhand.h"
#include "../R2/processManagement.h"
#include "time.h"
#define CLEAR_ALL "\033[2J"
#define MOVE_DEFAULT "\033[0;0H"

void getTimeCall(){
    date_time time = getTime();
    char sec[3]; //sec read by two digits and 3rd to terminate string
    itoa(time.sec,sec,10);
    char min[3]; //min read by two digits and 3rd to terminate string
    itoa(time.min,min,10);
    char hours[3]; //hour read by two digits and 3rd to terminate string   
    itoa(time.hour,hours,10);
    int size = 2;
    sys_req(WRITE, DEFAULT_DEVICE, hours, &size);
    size = 1;
    sys_req(WRITE, DEFAULT_DEVICE, ":", &size);
    size = 2;
   sys_req(WRITE, DEFAULT_DEVICE, min, &size);
    size = 1;
    sys_req(WRITE, DEFAULT_DEVICE, ":", &size);
    size = 2;
    sys_req(WRITE, DEFAULT_DEVICE, sec, &size);
}

void clear(){
    int size = 8;
    sys_req(WRITE, DEFAULT_DEVICE, CLEAR_ALL, &size);
    sys_req(WRITE, DEFAULT_DEVICE, MOVE_DEFAULT, &size);
}

/**
 * Function: version
 * -----------------------
 * Displays the current 
 * version of the project.
 * 
 * @author Bryce Williams
 */
void version()
{
    char versionR1[] = "MPX: MODULE R4 -VERSION 5.0";
    int versionR1Size = sizeof(versionR1) / sizeof(versionR1[0]);
    sys_req(WRITE, DEFAULT_DEVICE, versionR1, &versionR1Size);
}

void help2(){
   char helpManual2[] = 
   "\
    getTime  -Displays the user entered time.\n";
    
    int helpSize2 = sizeof(helpManual2) / sizeof(helpManual2[0]);
    sys_req(WRITE, DEFAULT_DEVICE, helpManual2, &helpSize2);
    
    char helpManual3[] = "\
    suspend  -Suspends user designated PCB.\n\
    resume   -Resumes user selected PCB that was previously suspended.\n";
    int helpSize3 = sizeof(helpManual3) / sizeof(helpManual3[0]);
    sys_req(WRITE, DEFAULT_DEVICE, helpManual3, &helpSize3);
}
void help3(){
    char helpManual5[] = 
    "\
    loadr3   -Loads the R3 processes in Suspended Ready with priority.\n\
    createAlarm  -Requests input for time and creates a new alarm.\n\
    allocate -Allocates memory of the given size.\n\
    free     -Frees memory at the given address.\n\
    isEmpty  -Confirms whether heap is empty or not\n";
    int helpSize5 = sizeof(helpManual5) / sizeof(helpManual5[0]);
    sys_req(WRITE, DEFAULT_DEVICE, helpManual5, &helpSize5);
}
void help4(){
    char helpManual6[] ="\
    showPCB  -Displays all information about user specified PCB.\n\
    showReady -Displays information of each PCB in the ready Queue.\n\
    showBlocked -Displays information of each PCB in the blocked Queue.\n\
    showAll  -Displays information of each PCB in the blocked and ready Queue.\n";
    int helpSize6 = sizeof(helpManual6) / sizeof(helpManual6[0]);
    sys_req(WRITE, DEFAULT_DEVICE, helpManual6, &helpSize6);
}
void help5(){
    char helpManual7[] ="\
    showFree  -Lists out all of the free memory blocks.\n\
    showAllocated  -Lists out all of the allocated memory blocks.\n";
    int helpSize7 = sizeof(helpManual7) / sizeof(helpManual7[0]);
    sys_req(WRITE, DEFAULT_DEVICE, helpManual7, &helpSize7);
}
/**
 * Function: help
 * -----------------------
 * print statement of 
 * the current 
 * commands that are 
 * executable. Tells user 
 * what commands do.
 * 
 * @author Bryce Williams
 */
void help()
{
    char helpManual[] =
    "To Run a Command Type Any of the Listed Commands:\n\
    help     -Lists the commands available for execution.\n\
    version  -Displays the current version of the MPX project.\n\
    shutdown -Shuts down the operating system.\n\
    setDate  -Set the current date.\n\
    getDate  -Displays the user set date.\n\
    setTime  -Sets the user entered time.\n";

    int helpSize = sizeof(helpManual) / sizeof(helpManual[0]);
    sys_req(WRITE, DEFAULT_DEVICE, helpManual, &helpSize);

    
    help2();
    
    help4();
    
    help3();
    
    help5();
}


/**
 * Function: shutdown
 * -----------------------
 * Shuts downt the comhand
 * by asking the user
 * for a confirmation first
 * 
 * @author Farhan Shahbaz
 */
int shutdown()
{
    char confirmation[] = "Are you sure you want to shutdown, Y for yes and N for no\n";
    int size = sizeof(confirmation) / sizeof(confirmation[0]);
    sys_req(WRITE, DEFAULT_DEVICE, confirmation, &size);
    char input[5];
    int inputSize = 4;
    //every value input in memset to be terminating string char
    memset(input, '\0', 5);
    sys_req(READ, DEFAULT_DEVICE, input, &inputSize);
    int result = strcasecmp("Y", input);
    if (result == 0)
    {
        sys_req(WRITE, DEFAULT_DEVICE, "\n", &inputSize);
        printWhite();
        return 1;
    }
    else
    {
        return 0;
    }
}

void showPCBCall(){
		char *message = "Enter the name of the process:\n";
		int size = 30;
		sys_req(WRITE, DEFAULT_DEVICE, message, &size);
		char name [20];
        memset(name, '\0', 20);
        size = 20;
        sys_req(READ, DEFAULT_DEVICE, name, &size);
        showPCB(name);
}

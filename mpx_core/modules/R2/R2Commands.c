#include "processManagement.h"
#include <string.h>
#include <system.h>
#include <core/serial.h>
#include "../mpx_supt.h"
#include <core/io.h>
#include "../R1/comhand.h"
#include "R2Commands.h"

void showReadyCall(){
    showReady();
}

void setPriorityCall(){
    //Remove 16,17,18 and replace with print ln message
    char *prompt = "Enter name of PCB to set the priority of:\n";
    int promptSize = sizeof(prompt) / sizeof(prompt[0]);
    sys_req(WRITE, DEFAULT_DEVICE, prompt, &promptSize);
    char name[10];
    int size = 9;
    memset(name, '\0', 10);
    sys_req(READ, DEFAULT_DEVICE, name, &size);
    char *pPrompt = "\nEnter priority value:\n";
    int pPromptSize = sizeof(pPrompt) / sizeof(pPrompt[0]);
    sys_req(WRITE, DEFAULT_DEVICE, pPrompt, &pPromptSize);
    char pValue[10];
    int pSize = 10;
    memset(pValue, '\0', 10);
    sys_req(READ, DEFAULT_DEVICE, pValue, &pSize);
    int userPValue = atoi(pValue);
    setPCBpriority(name,userPValue);
}

void deletePCBCall(){
    char *createPrompt = "Enter PCB Name:\n";
    int createPromptSize = 15;
    sys_req(WRITE, DEFAULT_DEVICE, createPrompt, &createPromptSize);
    char name[20];
    int size = 19;
    memset(name, '\0', 20);
    sys_req(READ, DEFAULT_DEVICE, name, &size);
    deletePCB(name);
}

// this isn't final bc no error checking
void createPCBCall(){
    printlnMessage("Enter PCB Name:");
    char name[10];
    int size = 9;
    memset(name, '\0', 10);
    sys_req(READ, DEFAULT_DEVICE, name, &size);
    printlnMessage("\nEnter PCB Priority:");
    char userP[9];
    memset(userP, '\0', 9);
    int inSize = 8;
    sys_req(READ, DEFAULT_DEVICE, userP, &inSize);
    int uPriority = atoi(userP);
    char userClass[5];
    memset(userClass, '\0', 5);
    int cSize = 5;
    printlnMessage("Enter PCB Class:[\nEnter 'S' for System 'A' for Application]");
    sys_req(READ, DEFAULT_DEVICE, userClass, &cSize);
    createPCB(name,userClass[0],uPriority);  
}

//This is a temporary command. How futile an existance.
void blockPCBCall(){
	char *targetPrompt = "Enter PCB Name:\n";
	int targetPromptSize = 16;
	sys_req(WRITE, DEFAULT_DEVICE, targetPrompt, &targetPromptSize);
    char name[10];
    int size = 9;
    memset(name, '\0', 10);
    sys_req(READ, DEFAULT_DEVICE, name, &size);
    blockPCB(name);
}

//This is also a temporary command. How also futile an also existance.
void unblockPCBCall(){
	char *targetPrompt = "Enter PCB Name:\n";
	int targetPromptSize = 16;
	sys_req(WRITE, DEFAULT_DEVICE, targetPrompt, &targetPromptSize);
	char name[10];
 int size = 9;
 memset(name, '\0', 10);
 sys_req(READ, DEFAULT_DEVICE, name, &size);
 unblockPCB(name);
}

void suspendPCBCall(){
    char *targetPrompt = "Enter PCB Name to suspend:\n";
    int targetPromptSize = 18;
    sys_req(WRITE, DEFAULT_DEVICE, targetPrompt, &targetPromptSize);
    char name[20];
    int size = 19;
    memset(name, '\0', 20);
    sys_req(READ, DEFAULT_DEVICE, name, &size);
    suspendPCB(name);
}

void resumePCBCall(){
    char *targetPrompt = "Enter PCB Name to resume:\n";
    int targetPromptSize = 18;
    sys_req(WRITE, DEFAULT_DEVICE, targetPrompt, &targetPromptSize);
    char name[20];
    int size = 19;
    memset(name, '\0', 20);
    sys_req(READ, DEFAULT_DEVICE, name, &size);
    resumePCB(name);
}
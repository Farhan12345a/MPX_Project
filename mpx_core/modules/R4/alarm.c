#include "alarm.h"
#include <system.h>
#include "../mpx_supt.h"
#include <string.h>
#include "../R1/time.h"
#include "list.h"
#include "../mpx_supt.h"

aList* head_alarms;

void createAList(){
    head_alarms = (aList*)kmalloc(sizeof(aList));
    head_alarms->head = NULL;
    head_alarms->tail = NULL;
    head_alarms->count = 0;
}
/**
* Function: timeDaemon
* -------------------------------
* Background process running to 
* check if an alarm has been set.
*
* @author Bryce Williams
*/
void timeDaemon()
{ 
	while(1){
		klogv("Time Daemon running");
		date_time time = getTime();
		alarm_t* current = head_alarms->head;
		while(current != NULL){
			if(time.hour == current->time.hour && time.min == current->time.min && (time.sec >= current->time.sec ||  time.sec <= current->time.sec)){ // convert to seconds
				printMessage("\n");
				printMessage(current->message);
				aListRemove(head_alarms,current);
			}
			current = current->next;
		}
		sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL); 
	}
}
/**
* Function: createAlarmCall
* -------------------------------
* Call to createAlarm that prompts
* user to enter alarm message/time.
* Sent to commhand.
* @author Farhan Shahbaz
*/
void createAlarmCall(){
	printlnMessage("Enter Alarm Message:");
	char message[30];
	int messsageSize = 29;
	memset(message,'\0',30);
	sys_req(READ, DEFAULT_DEVICE, message, &messsageSize);
	printlnMessage("\nEnter Alarm Time, Format hh:mm:ss");
	char userTime[20];
	int userTimeSize = 19;
	memset(userTime,'\0',20);
	sys_req(READ, DEFAULT_DEVICE, userTime, &userTimeSize);
	int hh = atoi(strtok(userTime,":"));
	int mm = atoi(strtok(NULL,":"));
	int ss = atoi(strtok(NULL,":"));
	if (hh < 0 || hh > 23 || mm < 0 || mm > 59 ||ss < 0 || ss > 59)
    {
        char errorHH[] = "\nInvalid time format: cannot create alarm";
        int errorHHsize = sizeof(errorHH) / sizeof(errorHH[0]);
        sys_req(WRITE, DEFAULT_DEVICE, errorHH, &errorHHsize);
        return;
    }
	createAlarm(hh,mm,ss,message);
}/**
* Function: createAlarm
* -------------------------------
* 
*
* @author Brendan Michael
*/
void createAlarm(int hh,int mm,int ss,char* message){
	alarm_t* alarm = (alarm_t*) sys_alloc_mem(sizeof(alarm_t));
	alarm->time.hour = hh;
	alarm->time.sec = ss;
	alarm->time.min = mm;
	memset(alarm->message,'\0',20);
	strcpy(alarm->message,message);
	aListInsert(head_alarms,alarm);
}

void infiniteProcess(){
	char msg[30];
    int count=0;	
	memset( msg, '\0', sizeof(msg));
	strcpy(msg, "\nINFINITE PROCESS EXECUTING.\n");
	count = strlen(msg);
  
  while(1){
	sys_req( WRITE, DEFAULT_DEVICE, msg, &count);
	asm volatile ("int $60"); //did not have to directly call int 60 could have not commented out below
 //   sys_req(IDLE, DEFAULT_DEVICE, NULL, NULL);
  }
}
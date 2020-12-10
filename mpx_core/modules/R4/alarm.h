#ifndef ALARM_H
#define ALARM_H
#include <system.h>
#include <mem/heap.h>

typedef struct alarm_s{
	struct alarm_s* next; 
	struct alarm_s* prev; 
	date_time time;
	char message[20];
}	alarm_t;

/**
 * Function: alarmCommand
 * -----------------------
 * Gathers and error checks
 * user input to create an
 * alarm.
 *
 * @author Bryce Williams
 */
void createAlarmCall();
/**
 * Function: createAlarm
 * -----------------------
 * Creates a new alarm based
 * on user specified time and 
 * adds it to the list of alarms
 * 
 * @param message: the message
 * to be displayed at the given
 * time
 * @author Brendan and Farhan
 */
void createAList();
void createAlarm(int hh,int mm,int ss,char* message);
void timeDaemon();
void infiniteProcess();
#endif
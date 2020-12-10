#ifndef LIST_H
#define LIST_H
#include "alarm.h"
#include <system.h>

typedef struct aList
{
	int count;
	alarm_t *head;
	alarm_t *tail;

} aList;

int aListInsert(aList *q, alarm_t *pcb);
int aListRemove(aList *q, alarm_t *pcb);
#endif
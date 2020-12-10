#include "list.h"
#include "../mpx_supt.h"
#include <string.h>
int aListInsert(aList *q, alarm_t *pcb){ 
    if(q->head == NULL){
        q->tail = pcb;
        q->head = q->tail;
        q->count++;
        return 1;
    }
    else{
        alarm_t *current = q->head;
        alarm_t *prev = NULL;
        while(current != NULL){
            prev = current;
            current = current->next;
        }
        //inserting at the head
        if(prev == NULL){
            pcb->next = current;
            current->prev = pcb;
            q->head = pcb;
        }
        //not inserting at the head
        else{
            pcb->next = current;
            pcb->prev = prev;
            prev->next = pcb;
            if(current != NULL){
                current->prev = pcb;
            }
            //inserting at the tail
            else{
                q->tail = pcb;

            }
        }
        q->count++;
    }
    return 1;
}

int aListRemove(aList *q, alarm_t *pcb){
    alarm_t *current = q->head;
    alarm_t *prev = NULL;
    while(current != NULL && strcmp(current->message,pcb->message) != 0){ // might be wrong
        prev = current;
        current = current->next;
    }

        //remove at the head
    if(prev == NULL){
        q->head = pcb->next;
        pcb->next = NULL;
        if(q->tail ==pcb){
            q->tail = NULL;
        }
    }
        //not removing at head
    else{
        if(current->next != NULL){
            prev->next = current->next;
            current->next->prev = prev;
        }
        else{
            current->prev = NULL;
            prev->next = NULL;
            q->tail = prev;
        }
        current->next = NULL;
        current->prev = NULL;
    }
    q->count--;
    return 1;

}
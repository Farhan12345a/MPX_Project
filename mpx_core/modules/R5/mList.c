#include "mList.h"
#include "../mpx_supt.h"

/**
 * Function: mListInsert
 * -----------------------
 * Inserts cmcb block into 
 * the list specified by q.
 * 
 * @param q-Pointer to the list.
 * @param cmcb- Pointer to the cmcb block to be inserted into the list. 
 * @authors Bryce Williams 
 */
//
int mListInsert(mList *q, CMCB_t *cmcb){ 
    if(q->head == NULL){ 
        q->tail = cmcb; 
        q->head = q->tail;
        q->count++;
        return 1;
    }
    else{ //inserting at the end of the list when the list is not empty
        CMCB_t *current = q->head;
        CMCB_t *prev = NULL;
        while(current != NULL){
            prev = current;
            current = current->next;
        }
        //inserting at the head
        if(prev == NULL){
            cmcb->next = current;
            current->prev = cmcb;
            q->head = cmcb;
        }
        //not inserting at the head
        else{
            cmcb->next = current;
            cmcb->prev = prev;
            prev->next = cmcb;
            if(current != NULL){
                current->prev = cmcb;
            }
            //inserting at the tail
            else{
                q->tail = cmcb;

            }
        }
        q->count++;
    }
    return 1;
}
/**
 * Function: mListInsert
 * -----------------------
 * Removes cmcb block from 
 * the list specified by q.
 * 
 * @param q-Pointer to the list.
 * @param cmcb- Pointer to the cmcb block to be removed from the list. 
 * @authors Bryce Williams 
 */
int mListRemove(mList *q, CMCB_t *cmcb){
    CMCB_t *current = q->head;
    CMCB_t *prev = NULL;
    while(current != NULL && current->start != cmcb->start){ //searching to find block we want to remove
        prev = current;
        current = current->next;
    }

        //remove at the head
    if(prev == NULL){
        q->head = cmcb->next;
        cmcb->next = NULL;
        if(q->tail ==cmcb){
            q->tail = NULL;
        }
    }
        //not removing at head
    else{
        if(current->next != NULL){
            prev->next = current->next;
            current->next->prev = prev;
        }
        else{ //removing at tail
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


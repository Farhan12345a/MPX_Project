#include "IOqueue.h"
#include <system.h>
#include "../R2/pcb.h"
ioqueue_t ioqueue ={NULL,NULL,0};

void enqueue(ioqueue_t* queue,iod_t* item){
    if(queue->tail == NULL){
        queue->head = item;
        queue->tail = item;
        
    } else{
        item->next = NULL;
        queue->tail->next = item;
        queue->tail = item;

    }
    queue->count++;
}
iod_t* dequeue(ioqueue_t* queue){
    iod_t* removed = NULL;
    if(queue->head == NULL){
        return NULL;
    } else{ 
        removed = queue->head;
        queue->head = removed->next;
        //removed only thing in queue
        if(queue->head == NULL){
            queue->tail = NULL;
        }
    }
    queue->count--;
    return removed;
}
iod_t* peek_IO(ioqueue_t* queue){
    return queue->head;
}

int isEmpty_IO(ioqueue_t* queue){
    return queue->head == NULL;
}


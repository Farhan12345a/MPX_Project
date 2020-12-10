#ifndef IO_QUEUE_H
#define IO_QUEUE_H
#include "../R2/pcb.h"
//Use s and t to distinguish between the two, so we do not confuse it later
typedef struct iod_s
{
    int op_code;
    //TODO: Will need to add ID to PCB... ask TA if its ok to use name vs ID
    char name[18];
    char *buffer_ptr;
    int *count_ptr;
    struct iod_s *next;
    PCB *pcb;
}iod_t;

typedef struct ioqueue_s
{
    iod_t* head;
    iod_t* tail;
    int count;
}ioqueue_t;
//adds element at the end of the queue (tail)
void enqueue(ioqueue_t* queue,iod_t* item);
//removes element from the head of the queue (head)
iod_t* dequeue(ioqueue_t* queue);
iod_t* peek_IO(ioqueue_t* queue);

int isEmpty_IO(ioqueue_t* queue);

#endif
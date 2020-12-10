#ifndef MLIST_H
#define MLIST_H
#include "memoryManagment.h"
#include <system.h>

typedef struct mList
{
      int count;
      CMCB_t *head;
      CMCB_t *tail;

} mList;

int mListInsert(mList *q, CMCB_t *pcb);
int mListRemove(mList *q, CMCB_t *pcb);

#endif
#include <stdio.h>
#include <stdlib.h>
#include "Queue.h"

typedef struct Qnode* link;

struct Qnode { 
    Pointer data; 
    link next; 
    link prev;
};

struct Queue{
    link head;
    link tail;
    DestroyFunc destroy;
    int size;
};

link CreateNode_qu(Pointer value, link next,link prev)
{ 
    link x = malloc(sizeof(*x));
    x->data = value;
    x->next = next;
    x->prev = prev;

    if(x->next != NULL)
        x->next->prev = x;

    return x;
}

QU QU_Init(DestroyFunc destroy)
{
    QU qu = malloc(sizeof(*qu));

    qu->head = NULL;
    qu->tail = NULL;
    qu->destroy = destroy;
    qu->size = 0;

    return qu;
}

int QU_Empty(QU qu)
{ 
    return qu->head == NULL; 
}

void QU_Enqueue(QU qu, Pointer value)
{
    qu->head = CreateNode_qu(value,qu->head,NULL);

    if(qu->tail == NULL)
        qu->tail = qu->head;
}

Pointer QU_Dequeue(QU qu)
{
    if (QU_Empty(qu))
        return NULL;
        
    Pointer value = qu->tail->data;
    link temp = qu->tail->prev;

    if(qu->tail == qu->head)
        qu->head = NULL;
        
    free(qu->tail);
    qu->tail = temp;
    
    return value;
}

void QU_Destroy(QU qu)
{
    link current = qu->head;
    link temp;

    while (current != NULL)
    {
        temp = current->next;

        if(qu->destroy != NULL)
            qu->destroy(current->data);

        free(current);

        current = temp;    
    }
    free(qu);
}

int* create_int(int x)
{
    int* y = malloc(sizeof(int));
    *y = x;
    return y;
}
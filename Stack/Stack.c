#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"

typedef struct STnode* link;

struct STnode { 
    Pointer data; 
    link next; 
};

struct Stack{
    link head;
    DestroyFunc destroy;
    int size;
};

link CreateNode(Pointer value, link next)
{ 
    link x = malloc(sizeof(*x));
    x->data = value;
    x->next = next;
    return x;
}

ST ST_Init(DestroyFunc destroy)
{
    ST st = malloc(sizeof(*st));

    st->head = NULL;
    st->destroy = destroy;
    st->size = 0;

    return st;
}

int ST_Empty(ST st)
{ 
    return st->head == NULL; 
}

void ST_Push(ST st, Pointer value)
{
    st->head = CreateNode(value,st->head);
}

Pointer ST_Pop(ST st)
{
    Pointer value = st->head->data;
    link temp = st->head->next;
    free(st->head);
    st->head = temp;
    
    return value;
}

void ST_destroy(ST st)
{
    link current = st->head;
    link temp;

    while (current != NULL)
    {
        temp = current->next;

        if(st->destroy != NULL)
            st->destroy(current->data);

        free(current);

        current = temp;    
    }
    free(st);
}
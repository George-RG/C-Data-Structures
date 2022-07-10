#pragma once

typedef void *Pointer; //Void pointer to store any kind of data needed
typedef void(*DestroyFunc)(Pointer); //Function to propertly free the momory that our data are using



typedef struct Queue* QU;

QU QU_Init(DestroyFunc);
int QU_Empty(QU);
void QU_Enqueue(QU, Pointer);
Pointer QU_Dequeue(QU);
void QU_Destroy(QU);

//Auxiliary functions
int* create_int(int);
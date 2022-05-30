typedef void *Pointer; //Void pointer to store any kind of data needed
typedef void(*DestroyFunc)(Pointer); //Function to propertly free the momory that our data are using

typedef struct Stack* ST;

ST ST_Init(DestroyFunc destroy);
int ST_Empty(ST st);
void ST_Push(ST st, Pointer value);
Pointer ST_Pop(ST st);
void ST_destroy(ST st);
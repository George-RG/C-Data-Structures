typedef void *Pointer;                              //Void pointer to store any kind of data needed
typedef void(*DestroyFunc)(Pointer);                //Function to propertly free the momory that our data are using (Set to NULL if not needed)
typedef int(*CompareFunc)(Pointer a,Pointer b);     //Returns 0 if a=b,<0 if a<b,>0 if a>b 
typedef void(*VisitFunc)(Pointer);                  //Function to use when traversing the tree

typedef struct priority_queue* PQ;                  //Pointer for the priority queue

// Inialize the priority queue data structure
// compare: Function to compare the priority of two nodes
// destroy: Function to free the memory of each data node stored in the queue
// Returns a pointer to the priority queue
PQ PQ_Initialize(CompareFunc compare, DestroyFunc destroy);

// Function to check if the queue is empty
// pq: Pointer to the priority queue
// Returns 1 if the queue is empty, 0 otherwise
int PQ_IsEmpty (PQ pq);

// Function to return the size of the queue
// pq: Pointer to the priority queue
// Returns the size of the queue
int PQ_Size(PQ pq);

// Function to insert a new node into the queue
// pq: Pointer to the priority queue
// data: Pointer to the data to be stored in the new node
void PQ_Insert(PQ pq, Pointer data);

// Function to remove the highest priority node from the queue
// pq: Pointer to the priority queue
// Returns the pointer to the data data stored in the highest priority node
Pointer PQ_Pop(PQ pq);

// Function to free the memory allocated for the queue
// pq: Pointer to the priority queue
// Note: The data stored in the queue will only be freed if the destroy function is not NULL
void PQ_Destroy(PQ pq);
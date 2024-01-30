#define RB_ERROR -1 // Error code
#define RB_OK 0     // Success code
#define RB_DUP 1    // Duplicate key error code

typedef void *Pointer; //Void pointer to store any kind of data needed
typedef void(*DestroyFunc)(Pointer); //Function to propertly free the momory that our data are using
typedef int(*CompareFunc)(Pointer a,Pointer b);//Returns 0 if a=b,<0 if a<b,>0 if a>b 

typedef struct redblack *RB; // Pointer to the entire tree

typedef struct _node *NodePtr;// Pointer to one of the tree's nodes

// Inizialize the tree data structure
RB RB_Initialize(DestroyFunc Destroy,CompareFunc Compare);

// Function to remove a key from the tree
// Returns -1 in case the key is not found 0 otherwise
int RB_RemoveKey(RB tree, Pointer key);

// Function to insert a key in the tree
// Returns -1 in case the key is already in the tree 0 otherwise
// No double keys are allowed in the tree in that case the function returns 1
int RB_InsertKey(RB tree, Pointer key);

// Function to search a key in the tree
// Returns a pointer to the key if found NULL otherwise
NodePtr RB_Search(RB tree, Pointer key);

// Function to print the tree in order
void RB_PrintElements(RB tree,void(*Visit)(Pointer));

// Function to extract the data from a node
Pointer RB_DataFromNode(NodePtr node);

// Returns the number of nodes in the tree
int RB_Size(RB tree);

// Prints a 2D representation of the tree
void RB_print2D(RB tree);

// Function to destroy the tree
// Frees all the memory used by the tree
void RB_Destroy(RB tree);

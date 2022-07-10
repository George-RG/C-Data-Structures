typedef void *Pointer; //Void pointer to store any kind of data needed
typedef void(*DestroyFunc)(Pointer); //Function to propertly free the momory that our data are using
typedef int(*CompareFunc)(Pointer a,Pointer b);//Returns 0 if a=b,<0 if a<b,>0 if a>b 
typedef void(*VisitFunc)(Pointer);//Function to use when traversing the tree

typedef struct redblack *RB; // Pointer to the entire tree

typedef struct _node *NodePtr;// Pointer to one of the tree's nodes


RB Initialize(DestroyFunc Destroy,CompareFunc Compare);
int RemoveKey(RB tree, Pointer key); //Returns -1 in case the key is not found
int InsertKey(RB tree, Pointer key); //Returns -1 in case the insertion failed
NodePtr Search(RB tree, Pointer key);
void PrintElements(RB tree,VisitFunc);
Pointer DataFromNode(NodePtr node);
void print2D(RB tree);
void RBFree(RB tree);
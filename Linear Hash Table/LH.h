typedef unsigned int uint;
typedef void* Pointer;

typedef enum{false,true} bool;

typedef void(*DestroyFunc)(Pointer); //Function to propertly free the momory that our data are using
typedef int(*CompareFunc)(Pointer a,Pointer b);//Returns 0 if a=b,<0 if a<b,>0 if a>b 
typedef uint(*HashFunc)(Pointer);//Returns a hash value for the given data

typedef struct lh* LH;
typedef struct node* Node;

#define INITIAL_CAPACITY 10
#define OVERFLOW_LIMIT 5
#define UNDERFLOW_LIMIT 2
#define MAX_LOAD_FACTOR 1.5

LH LH_Initialize(DestroyFunc destroy_key, DestroyFunc destroy_value, CompareFunc compare_key, HashFunc hash);

bool LH_Insert(LH lh, Pointer key, Pointer value);
bool LH_Remove(LH lh , Pointer key);
Node LH_Find(LH lh, Pointer key);

void LH_destory(LH lh);

int LH_size(LH lh);
void LH_set_hash_function(LH lh, HashFunc hash_function);

Pointer LH_Get_Value(LH lh, Node node);
Pointer LH_Get_Key(LH lh, Node node);

void LH_Print(LH lh);
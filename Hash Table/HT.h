#include <stdio.h>
#include <stdlib.h>

#define MAX_LOAD 0.5
#define START_BUCKETS 1000

typedef unsigned int natural_num;
typedef char byte;// Used tp return 0 for success and -1 for falure 
typedef void *Pointer; //Void pointer to store any kind of data needed

typedef void(*DestroyFunc)(Pointer); //Function to propertly free the momory that our data are using
typedef int(*HashFunc)(Pointer a);//Hash function to convert data to a spesific int
typedef int(*CompareFunc)(Pointer a,Pointer b);//Returns 0 if a=b,<0 if a<b,>0 if a>b 

typedef struct _ht* HT;

HT ht_initialize(CompareFunc, DestroyFunc, HashFunc);
int ht_size(HT);
byte ht_insert(HT, Pointer);
byte ht_remove(HT, Pointer);
Pointer ht_find(HT, Pointer);
DestroyFunc hash_set_destroy (HT, DestroyFunc);
void ht_destroy(HT);


/*
Examples of hash functions:

uint hash_string(Pointer value) {
	// djb2 hash function, απλή, γρήγορη, και σε γενικές γραμμές αποδοτική
    uint hash = 5381;
    for (char* s = value; *s != '\0'; s++)
		hash = (hash << 5) + hash + *s;			// hash = (hash * 33) + *s. Το foo << 5 είναι γρηγορότερη εκδοχή του foo * 32.
    return hash;
}

uint hash_int(Pointer value) {
	return *(int*)value;
}

uint hash_pointer(Pointer value) {
	return (size_t)value;				// cast σε sizt_t, που έχει το ίδιο μήκος με έναν pointer
}

*/
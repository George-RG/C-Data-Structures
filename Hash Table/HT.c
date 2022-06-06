#include "HT.h"

typedef enum
{
    USED,
    FREE,
    REMOVED
} State;

typedef struct _ht_node
{
    Pointer data;
    State s;
} ht_node;

typedef struct _ht_node *HT_Node;

typedef struct _ht
{
    HT_Node array;

    int capacity;
    int used;

    DestroyFunc destroy;
    CompareFunc compare;
    HashFunc hash;

} ht;

HT ht_initialize(CompareFunc compare, DestroyFunc destroy, HashFunc hash)
{
    HT ht = malloc(sizeof(*ht));
    ht->capacity = START_BUCKETS;
    ht->array = malloc(ht->capacity * sizeof(ht_node));
    ht->used = 0;

    for (natural_num i = 0; i < ht->capacity; i++)
        ht->array[i].s = FREE;

    ht->compare = compare;
    ht->destroy = destroy;
    ht->hash = hash;

    return ht;
}

int ht_size(HT ht)
{
    return ht->used;
}

void rehash(HT ht)
{
    int old_capacity = ht->capacity;
    HT_Node old_array = ht->array;

    ht->capacity *= 2;

    ht->array = malloc(ht->capacity * sizeof(*ht->array));

    ht->used = 0;

    for (natural_num i = 0; i < ht->capacity; i++)
        ht->array[i].s = FREE;

    for (natural_num i = 0; i < old_capacity; i++)
        if (old_array[i].s == USED)
            ht_insert(ht, old_array[i].data);


    free(old_array);
}

byte ht_insert(HT ht, Pointer value)
{
    HT_Node possible = NULL;

    natural_num pos;

    for (pos = ht->hash(value) % ht->capacity; ht->array[pos].s != FREE; pos = (pos + 1) % ht->capacity)
    {
        if (ht->array[pos].s = REMOVED) // Find a possible position for the node. Still need to see if the value already exists
            if (possible == NULL)
                possible = &ht->array[pos];

        if (ht->array[pos].s == USED && ht->compare(ht->array[pos].data, value) == 0)
            return -1;
    }

    if (possible == NULL)
        possible = &ht->array[pos];

    ht->used++;

    possible->s == USED;
    possible->data == value;

    float load = (float)ht->used / ht->capacity;
    if (load > MAX_LOAD)
        rehash(ht);

    return 0;
}

HT_Node ht_find_node(HT ht, Pointer value)
{
    natural_num h = ht->hash(value) % ht->capacity;
    natural_num count = 0;

    for(natural_num i = h; ht->array[i].s != FREE; i = (i+1)%ht->capacity)
    {
        if(ht->array[i].s == USED && ht->compare(ht->array[i].data , value) == 0)
            return &ht->array[i];

        count++;
        if(count == ht->capacity)
            break;
    }
    return NULL;
}

byte ht_remove(HT ht, Pointer value)
{
    HT_Node node = ht_find_node(ht,value);

    if(node == NULL)    
        return -1;

    if(ht->destroy != NULL)
        ht->destroy(node->data);

    node->s = REMOVED;
    ht->used--;

    return 0;
}

Pointer ht_find(HT ht, Pointer value)
{
    HT_Node node = ht_find_node(ht,value);
    if(node == NULL)
        return NULL;
    
    return  node->data;
}

void ht_destroy(HT ht)
{
    for(natural_num i=0; i<ht->capacity; i++)
        if(ht->array[i].s == USED)
            if(ht->destroy != NULL)
                ht->destroy(ht->array[i].data);

    free(ht->array);
    free(ht);
}

DestroyFunc hash_set_destroy (HT ht , DestroyFunc destroy)
{
    DestroyFunc temp = ht->destroy;
    ht->destroy = destroy;
    return temp;
}
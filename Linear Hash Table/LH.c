#include <stdio.h>
#include <stdlib.h>

#include "LH.h"

struct node {
    Pointer key;
    Pointer value;
};

typedef struct node* Node;

struct list_node {
    Node data;

    struct list_node* next;
};

typedef struct list_node* ListNode;

struct hash_node
{
   Node* array;
   ListNode overflow;

   int array_size;

   bool split;
};

typedef struct hash_node* HashNode;

struct lh
{
    HashNode table;
    HashNode next_table;
    uint size;
    uint capacity;
    uint next_split_index;

    DestroyFunc destroy_key;
    DestroyFunc destroy_value;
    CompareFunc compare_key;
    HashFunc hash;
};

bool LH_Insert_helper(LH lh, Pointer key, Pointer value, bool from_split);

void free_node(LH lh,Node node)
{
    if(lh->destroy_key != NULL)
    {
        lh->destroy_key(node->key);
    }
    if(lh->destroy_value != NULL && node->value != NULL)
    {
        lh->destroy_value(node->value);
    }
    free(node);
}

Node copy_node(Node node)
{
    Node new_node = malloc(sizeof(struct node));
    new_node->key = node->key;
    new_node->value = node->value;
    return new_node;
}

int LH_size(LH lh)
{
    return lh->size;
}

LH LH_Initialize(DestroyFunc destroy_key, DestroyFunc destroy_value, CompareFunc compare_key, HashFunc hash)
{
    uint capacity = INITIAL_CAPACITY;

    LH lh = malloc(sizeof(struct lh));
    if (lh == NULL)
    {
        return NULL;
    }
    lh->table = malloc(capacity * sizeof(struct hash_node));
    if (lh->table == NULL)
    {
        free(lh);
        return NULL;
    }

    for(uint i = 0; i < capacity; i++)
    {
        lh->table[i].array = NULL;
        lh->table[i].overflow = NULL;
        lh->table[i].array_size = 0;
        lh->table[i].split = false;
    }

    lh->next_table = NULL;
    lh->size = 0;
    lh->next_split_index = 0;
    lh->capacity = capacity;
    lh->destroy_key = destroy_key;
    lh->destroy_value = destroy_value;
    lh->compare_key = compare_key;
    lh->hash = hash;
    return lh;
}

void end_of_round(LH lh)
{

    for(int i=0; i<lh->capacity; i++)
        if(lh->table[i].array != NULL)
            free(lh->table[i].array);

    free(lh->table);

    lh->table = lh->next_table;
    lh->next_table = NULL;
    lh->capacity = lh->capacity * 2;
    lh->next_split_index = 0;
}

void split(LH lh)
{
    if(lh->next_table == NULL)
    {
        lh->next_table = malloc(2 * lh->capacity * sizeof(struct hash_node));
        if (lh->next_table == NULL)
            return;

        for(int i=0; i<lh->capacity * 2; i++)
        {
            lh->next_table[i].array = NULL;
            lh->next_table[i].overflow = NULL;
            lh->next_table[i].array_size = 0;
            lh->next_table[i].split = false;
        }
    }


    HashNode old_block = &lh->table[lh->next_split_index];
    old_block->split = true;
    int old_size = old_block->array_size;
    old_block->array_size = 0;
    for(int i = 0; i< old_size; i++)
    {
        lh->size--;

        LH_Insert_helper(lh,old_block->array[i]->key, old_block->array[i]->value,true);
        free(old_block->array[i]);
        old_block->array[i] = NULL;
    }


    ListNode cur = old_block->overflow;
    ListNode temp;
    old_block->overflow = NULL;
    while(cur != NULL)
    {
        lh->size--;
        LH_Insert_helper(lh, cur->data->key, cur->data->value,true);
        temp = cur;
        cur = cur->next;
        free(temp->data);
        free(temp);
    }

    lh->next_split_index++;
    if(lh->next_split_index == lh->capacity)
    {
        end_of_round(lh);
    }
}

bool LH_Insert_helper(LH lh, Pointer key, Pointer value, bool from_split)
{
    bool force_split = false;
    bool inserted = false;

    uint hash = lh->hash(key);
    uint pos = hash % lh->capacity;
    HashNode block = &lh->table[pos];

    if (block->split == true)
    {
        pos = hash % (lh->capacity * 2);
        block = &lh->next_table[pos];
    }

    Node node = malloc(sizeof(*node));
    node->key = key;
    node->value = value;

    if (block->array == NULL)
    {
        block->array = malloc(OVERFLOW_LIMIT * sizeof(Node));
        if (block->array == NULL)
            return false;

        block->overflow = NULL;
        block->split = false;
        block->array_size = 1;

        block->array[0] = node;

        inserted = true;
    }
    else if (block->array != NULL)
    {
        for(int i=0; i<block->array_size; i++)
        {
            if (lh->compare_key(block->array[i]->key, key) == 0)
            {

                free_node(lh,block->array[i]);

                block->array[i] = node;

                inserted = true;
                return inserted;
            }
        }

        if(block->array_size < OVERFLOW_LIMIT)
        {
            block->array[block->array_size] = node;
            block->array_size++;

            inserted = true;
        }
    }

    if (block->array_size == OVERFLOW_LIMIT && inserted == false)
    {
        bool replaced = false;

        ListNode temp = block->overflow;
        while (temp != NULL)
        {
            if (lh->compare_key(temp->data->key, key) == 0)
            {
                free_node(lh,temp->data);
                temp->data == node;
                replaced = true;
                lh->size--;
                break;
            }
            temp = temp->next;
        }

        if (replaced == false)
        {    
            ListNode new_node = malloc(sizeof(*new_node));
            if (new_node == NULL)
                return false;

            new_node->data = node;
            new_node->next = block->overflow;
            block->overflow = new_node;
        } 
        
        force_split = true;
        inserted = true;
    }

    if(inserted)
        lh->size++;

    //float load_factor = ((float)lh->size / OVERFLOW_LIMIT) / ((float)lh->capacity + (float)lh->next_split_index);
    if(/* load_factor > MAX_LOAD_FACTOR ||*/ force_split == true && from_split == false)
    {
        split(lh);
    }

    return inserted;
}

Node LH_Find(LH lh, Pointer key)
{
    uint hash = lh->hash(key);
    uint pos = hash % lh->capacity;
    HashNode block = &lh->table[pos];

    if (block->split == true)
    {
        pos = hash % (lh->capacity * 2);
        block = &lh->next_table[pos];
    }

    if (block->array == NULL)
        return NULL;

    for(int i=0; i<block->array_size; i++)
    {
        if (lh->compare_key(block->array[i]->key, key) == 0)
        {
            return block->array[i];
        }
    }

    ListNode temp = block->overflow;
    while (temp != NULL)
    {
        if (lh->compare_key(temp->data->key, key) == 0)
        {
            return temp->data;
        }
        temp = temp->next;
    }

    return NULL;
}

bool LH_Insert(LH lh, Pointer key, Pointer value)
{
    return LH_Insert_helper(lh, key, value, false);
}

bool LH_Remove(LH lh , Pointer key)
{
    uint hash = lh->hash(key);
    uint pos = hash % lh->capacity;
    HashNode block = &lh->table[pos];

    if (block->split == true)
    {
        pos = hash % (lh->capacity * 2);
        block = &lh->next_table[pos];
    }

    if (block->array == NULL)
        return false;

    for(int i=0; i<block->array_size; i++)
    {
        if (lh->compare_key(block->array[i]->key, key) == 0)
        {
            free_node(lh,block->array[i]);

            if(i != block->array_size - 1)
            {
                block->array[i] = block->array[block->array_size - 1];
            }
            block->array[block->array_size - 1] = NULL;

            block->array_size--;

            if (block->overflow != NULL)
            {
                block->array[block->array_size] = block->overflow->data;
                block->array_size++;

                ListNode temp = block->overflow;
                block->overflow = block->overflow->next;
                free(temp);
            }

            lh->size--;
            return true;
        }
    }

    ListNode temp = block->overflow;
    ListNode prev = NULL;
    while (temp != NULL)
    {
        if (lh->compare_key(temp->data->key, key) == 0)
        {
            if (prev == NULL)
                block->overflow = temp->next;
            else
                prev->next = temp->next;
            free_node(lh,temp->data);
            free(temp);
            lh->size--;
            return true;
        }
        prev = temp;
        temp = temp->next;
    }

    return false;
}

void LH_set_hash_function(LH lh, HashFunc hash_function)
{
    lh->hash = hash_function;
}

void LH_destory(LH lh)
{
    for(int i=0; i<lh->capacity; i++)
    {
        if (lh->table[i].array != NULL)
        {
            for(int j=0; j<lh->table[i].array_size; j++)
            {
                if(lh->table[i].array[j] != NULL)
                    free_node(lh,lh->table[i].array[j]);
            }
            free(lh->table[i].array);
        }
        if (lh->table[i].overflow != NULL)
        {
            ListNode temp = lh->table[i].overflow;
            while (temp != NULL)
            {
                ListNode temp2 = temp;
                temp = temp->next;
                free_node(lh,temp2->data);
                free(temp2);
            }
        }
    }
    if(lh->next_table != NULL)
    {
        for(int i=0; i<lh->capacity * 2; i++)
        {
            if (lh->next_table[i].array != NULL)
            {
                for(int j=0; j<lh->next_table[i].array_size; j++)
                {
                    if(lh->next_table[i].array[j] != NULL)
                        free_node(lh,lh->next_table[i].array[j]);
                }
                free(lh->next_table[i].array);
            }
            if (lh->next_table[i].overflow != NULL)
            {
                ListNode temp = lh->next_table[i].overflow;
                while (temp != NULL)
                {
                    ListNode temp2 = temp;
                    temp = temp->next;
                    free_node(lh,temp2->data);
                    free(temp2);
                }
            }
        }
    }
    free(lh->table);
    free(lh->next_table);
    free(lh);
}

Pointer LH_Get_Value(LH lh,Node node)
{
    if(node == NULL)
        return NULL;
    return node->value;
}

Pointer LH_Get_Key(LH lh,Node node)
{
    if(node == NULL)
        return NULL;
    return node->key;
}

void LH_Print(LH lh)
{
    printf("Main Table:\n");
    for(int i=0; i<lh->capacity; i++)
    {
        if(lh->table[i].split == true)
            printf("%d Split:",i);
        else
            printf("%d Not Split:",i);

        if (lh->table[i].array != NULL)
        {
            for(int j=0; j<lh->table[i].array_size; j++)
            {
                if(lh->table[i].array[j] != NULL)
                    printf("(%d,%d) ",*(int*)lh->table[i].array[j]->key,*(int*)lh->table[i].array[j]->value);
            }
        }
        if (lh->table[i].overflow != NULL)
        { 
            printf(" -----> ");
            ListNode temp = lh->table[i].overflow;
            while (temp != NULL)
            {
                printf("(%d,%d) ",*(int*)temp->data->key,*(int*)temp->data->value);
                temp = temp->next;
            }
        }
        printf("\n");
    }
    if(lh->next_table != NULL)
    {
        printf("Next Table:\n");
        for(int i=0; i<lh->capacity * 2; i++)
        {
            if(lh->next_table[i].split == true)
                printf("%d Split:",i);
            else
                printf("%d Not Split:",i);

            if (lh->next_table[i].array != NULL)
            {
                for(int j=0; j<lh->next_table[i].array_size; j++)
                {
                    if(lh->next_table[i].array[j] != NULL)
                        printf("(%d,%d) ",*(int*)lh->next_table[i].array[j]->key,*(int*)lh->next_table[i].array[j]->value);
                }
            }
            if (lh->next_table[i].overflow != NULL)
            {
                printf(" -----> ");
                ListNode temp = lh->next_table[i].overflow;
                while (temp != NULL)
                {
                    printf("(%d,%d) ",*(int*)temp->data->key,*(int*)temp->data->value);
                    temp = temp->next;
                }
            }
            printf("\n");
        }
    }
    printf("Split Index: %d\n",lh->next_split_index);
    printf("////////////////////////////////////////////////////////////////////////////////////////////\n");
}
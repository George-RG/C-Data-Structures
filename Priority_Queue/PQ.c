#include <stdio.h>
#include <stdlib.h>

#include "PQ.h"

#define DEBUG 1

// Structs for min heap tree
typedef struct heap_node
{
    Pointer data;
    int min_hight;

    struct heap_node *left;
    struct heap_node *right;
    struct heap_node *parent;

} heap_node;

typedef heap_node *HeapNode;

// Struct for PQ
typedef struct priority_queue
{
    HeapNode root;

    int size;
    int layer;
    int cur_max_index;

    HeapNode *last_inserted;

    CompareFunc compare;
    DestroyFunc destroy;
} priority_queue;

PQ PQ_Initialize(CompareFunc compare, DestroyFunc destroy)
{
    priority_queue *pq = malloc(sizeof(priority_queue));

    pq->root = NULL;
    pq->size = 0;
    pq->layer = 0;
    pq->cur_max_index = 0;

    pq->compare = compare;
    pq->destroy = destroy;
    return pq;
}

int PQ_IsEmpty(PQ pq)
{
    return pq->size == 0;
}


int PQ_Size(PQ pq)
{
    return pq->size;
}

HeapNode Insert_Helper(PQ pq, int height, HeapNode root, HeapNode node)
{
    if(height > pq->layer)
    {
        return NULL;
    }

    if(root->left == NULL)
    {
        root->left = node;
        root->left->parent = root;

        int temp1 = *((int*)root->left->data);
        int temp2 = *((int*)root->data);

        if(pq->compare(root->left->data, root->data) > 0)
        {
            Pointer temp = root->data;
            root->data = root->left->data;
            root->left->data = temp;
        }

        temp1 = *((int*)root->left->data);
        temp2 = *((int*)root->data);

        return root;
    }
    else if(root->right == NULL)
    {
        root->right = node;
        root->right->parent = root;

        if(pq->compare(root->right->data, root->data) > 0)
        {
            Pointer temp = root->data;
            root->data = root->right->data;
            root->right->data = temp;
        }

        root->min_hight = 1;

        return root;
    }
    else
    {
        if(height + root->left->min_hight < pq->layer)
        {
            root->left = Insert_Helper(pq, height + 1, root->left, node);

            //Heapify the tree
            if(pq->compare(root->left->data, root->data) > 0)
            {
                Pointer temp = root->data;
                root->data = root->left->data;
                root->left->data = temp;
            }
        }
        else
        {
            root->right = Insert_Helper(pq, height + 1, root->right, node);

            //Heapify the tree  
            if(pq->compare(root->right->data, root->data) > 0)
            {
                Pointer temp = root->data;
                root->data = root->right->data;
                root->right->data = temp;
            }
        }
        
        // Update min_hight
        if(root->left->min_hight < root->right->min_hight)
        {
            root->min_hight = root->left->min_hight + 1;
        }
        else
        {
            root->min_hight = root->right->min_hight + 1;
        }
    }
}

void Fix_Layer(PQ pq)
{
    if(DEBUG)
        printf("Fixing layer size:%d, max:%d\n", pq->size, pq->cur_max_index);
    
    if(pq->size == pq->cur_max_index)
    {
        pq->layer++;
        pq->cur_max_index += (1 << pq->layer);
    }
}

//Heap Insertion
void PQ_Insert(PQ pq, Pointer data)
{
    HeapNode new_node = malloc(sizeof(heap_node));
    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;
    new_node->parent = NULL;
    new_node->min_hight = 0;

    if (pq->root == NULL)
    {
        pq->root = new_node;
        pq->layer = 1;
        pq->cur_max_index = 3;
    }
    else
    {
        pq->root = Insert_Helper(pq, 1, pq->root, new_node);
    }

    pq->size++;
    Fix_Layer(pq);
    pq->last_inserted = new_node;
}

void PQ_Pop(PQ pq)
{

}

void ShowHeap(HeapNode node, int space)
{
    if (node == NULL) return;  // Base case
 
    // Increase distance between levels
    space += 8;

    ShowHeap(node->right, space);
 
    printf("\n");
    for (int i = 8; i < space; i++)
        printf(" ");


    printf("%d(%d)\n", *((int*)node->data) , node->min_hight);

    // Process left child
    ShowHeap(node->left, space);
}

int compare_ints(Pointer a, Pointer b)
{
    return *((int*)a) - *((int*)b);
}

int main(void)
{
    //Insert 10 elements 

    PQ pq = PQ_Initialize(compare_ints, free);

    int k=712;

    for(int i = 0; i < 11; i++)
    {
        int *data = malloc(sizeof(int));
        // if(i % 2)
        // {
        //     int temp = i % 10;
        //     temp = 0 - temp;
        //     *data = temp;
        // }
        // else
        // {
            *data = k;
        // }

        PQ_Insert(pq, data);
        ShowHeap(pq->root, 0);

        printf("\nInserted %d\n", *data);
        k=(401*k+417)%1000;
    }
}
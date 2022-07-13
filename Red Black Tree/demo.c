#include <stdio.h>
#include <stdlib.h>
#include "RB.h"

int* CreateInt(int input)
{
    int *result = malloc(sizeof(*result));
    *result = input;
    return result;
}

int CompInts(Pointer a,Pointer b)
{
    int* first = (int*)a;
    int* second = (int*)b;
    return(*first - *second);
}

void PrintNode(Pointer a)
{
    printf("%d ",*((int*)a));
}

int main(void)
{
    RB tree = Initialize(free,CompInts);
    NodePtr TempNode;
    int operation;
    int key;
    int result;
    int from,to;


    printf("\nChoose operation: \n1.Insert Key \n2.Remove Key \n3.Shearch for a Key \n4.Print all the keys in ascending order \n5.Print currently sotored key \n6.Visualize tree \n7.Destroy current try and create new \n8.Insert range of numbers \nType 'quit' to exit\n");
    while(scanf(" %d",&operation) != 0)
    {
        switch (operation)
        {
            case 1:
                printf("Type the key you wish to insert: ");
                scanf(" %d",&key);

                result = InsertKey(tree,CreateInt(key));

                if(result == -1)
                    printf("Key %d already exists\n",key);
                else
                    printf("Key %d inserted succesfully\n",key);

                break;

            case 2:
                printf("Type the key you wish to remove: ");
                scanf(" %d",&key);

                result = RemoveKey(tree,&key);
                
                if(result == -1)
                    printf("Key %d already exists\n",key);
                else
                    printf("Key %d removed succesfully\n",key);

                break;
            
            case 3:
                printf("Type the key you wish to insert: ");
                scanf(" %d",&key);

                TempNode = Search(tree,CreateInt(key));
                if(TempNode != NULL)
                    printf("Key %d found succesfully and stored localy\n",key);
                else
                    printf("Key %d not found\n",key);

                break;
            
            case 4:
                PrintElements(tree,PrintNode);
                break;

            case 5:
                PrintNode(DataFromNode(TempNode));
                break;

            case 6:
                print2D(tree);
                break;

            case 7:
                RBFree(tree);
                tree = Initialize(free,CompInts);
                break;

            case 8:
                printf("Insert numbers from: ");
                scanf(" %d",&from);

                printf("Insert numbers to: ");
                scanf(" %d",&to);

                for(int i=from; i<=to;i++)
                    InsertKey(tree,CreateInt(i));

                break;

            default:
                break;
        }
    printf("\nChoose operation: \n1.Insert Key \n2.Remove Key \n3.Shearch for a Key \n4.Print all the keys in ascending order \n5.Print currently sotored key \n6.Visualize tree \n7.Destroy current try and create new \n8.Insert range of numbers \nType 'quit' to exit\n");
    }

    RBFree(tree);
}
#ifndef BTREE_H
#define BTREE_H
#include "../linked_list/list.h"
#include <pthread.h>


typedef struct node
{
    int integer;
    struct node *left, *right;

} Node;

//A binary tree with a mutex
typedef struct btree
{
    pthread_mutex_t mutex;
    Node *root;
} BTree;

BTree *buildBTree();

//Inserts an integer into the tree, blocking method
Node *insert(BTree *tree, int integer);

//Returns the maximum value of the tree, blocking method
int getMax(BTree *tree);

List *getAllIntegers(BTree *tree);

//Shows me if the tree looks how it should
//Prints tree in traditional sorted order, blocking method
void printTree(BTree *tree);

void destroyTree(BTree *tree);

#endif // !BTREE_H
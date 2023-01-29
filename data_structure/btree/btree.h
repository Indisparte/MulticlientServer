#ifndef BTREE_H
#define BTREE_H
#include "../linked_list/list.h"
#include <pthread.h>

// A structure to represent node of kd tree
// 0 latitude and 1 longitude
typedef struct node
{
    int integer;
    struct node *left, *right;

} Node;

typedef struct kdtree
{
    pthread_mutex_t mutex;
    Node *root;
} BTree;

BTree *buildBTree();
Node *insert(BTree *tree, int integer);
int getMax(BTree *tree);

/*
Searches through root for points inside given radius.
If a node is inside the radius, is added to a result list.
*/
List *getAllIntegers(BTree *tree);
/*
Shows me if my tree looks how it should during testing
Prints tree in traditional sorted order.
*/
void printTree(BTree *tree);
void destroyTree(BTree *tree);

#endif // !BTREE_H
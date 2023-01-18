#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../util/util.h"
#include <math.h>
#include <string.h>
#include <stdbool.h>

BTree *newTree()
{
    KDTree *result = malloc(sizeof(KDTree));
    result->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    result->root = NULL;
    return result;
}

BTree *buildKDTreeFromFile(const char *filename)
{
    BTree *tree = newTree();
    FILE *file = fopen(filename, "r");
    assert(file);

    if (file != NULL)
    {
        printf("[+]Building tree...\n");
        char *currentLine = NULL;
        size_t strLen = 0;
        char insideBrachet[BUFSIZ];

        while (getline(&currentLine, &strLen, file) != -1)
        {
            stringInsideSquareBracket(currentLine, sizeof(insideBrachet), insideBrachet);
            int value = atoi(insideBrachet);

            tree->root = insert(tree, value);
        }

        free(currentLine);
        fclose(file);
    }
    printf("[+]Tree built\n");

    return tree;
}

/*
Opens storage file (data/data.txt), uses it to build kd tree, then closes file and returns pointer to the tree.
*/
BTree *buildKDTree()
{
    return buildKDTreeFromFile("data/data.txt");
}

Node *newNode(int *integer)
{
    Node *temp = malloc(sizeof(Node));

    temp->integer = integer;

    temp->left = temp->right = NULL;
    return temp;
}

int getMaxRec(Node *node)
{
    if (node->right == NULL)
        return node->data;
    return getMaxRec(node->right);
}

int *getMax(BTree *tree)
{
    pthread_mutex_lock(&tree->mutex);
    int *result = getMaxRec(tree->root);
    pthread_mutex_unlock(&tree->mutex);

    return result;
}

Node *insertRec(Node *node, int *data)
{
    /* 1. If the tree is empty, return a new,
     single node */
    if (node == NULL)
        return newNode(data);
    else
    {

        /* 2. Otherwise, recur down the tree */
        if (data <= node->data)
            node->left = insert(node->left, data);
        else
            node->right = insert(node->right, data);

        /* return the (unchanged) node pointer */
        return node;
    }
}

Node *insert(BTree *tree, int *integer)
{
    pthread_mutex_lock(&tree->mutex);
    Node *result = insertRec(tree->root, integer);
    pthread_mutex_unlock(&tree->mutex);

    return result;
}

void freeTree(Node *node)
{
    if (node)
    {
        freeTree(node->left);
        freeTree(node->right);
        free(node);
    }
}

void destroyTree(BTree *tree)
{
    freeTree(tree->root);
    pthread_mutex_destroy(&tree->mutex);
}

// Ticker is used to alternate key comparisons between latitude and longitude
void getAsList(Node *root, List *list)
{

    if (root == null)
        return;

    getAsList(node->left, list);

    if (root->left == null && node->right == null)
    {
        addToList(root->integer, list);
        return;
    }

    getAsList(node->right, list);
}

// is thread safe.
List *getAllIntegers(BTree *tree)
{
    List *list = makelist();
    pthread_mutex_lock(&tree->mutex);
    getAsList(tree->root, list);
    pthread_mutex_unlock(&tree->mutex);
    return list;
}

// Function to print binary tree in 2D
// It does reverse inorder traversal
void print2DUtil(Node *root, int space)
{
    // Base case
    if (root == NULL)
        return;

    // Increase distance between levels
    space += 10;

    // Process right child first
    print2DUtil(root->right, space);

    // Print current node after space
    // count
    printf("\n");
    for (int i = 10; i < space; i++)
        printf(" ");
    printf("%s [%f,%f]\n", root->pothole->user->username, root->pothole->latitude, root->pothole->longitude);

    // Process left child
    print2DUtil(root->left, space);
}

/*
Prints tree in traditional sorted order locking mutex
*/
void printTree(BTree *tree)
{
    pthread_mutex_lock(&tree->mutex);
    if (tree->root == NULL)
    {
        printf("[-]Tree is empty\n");
        pthread_mutex_unlock(&tree->mutex);
        return;
    }
    // treeprint(tree->root,0);
    print2DUtil(tree->root, 0);
    pthread_mutex_unlock(&tree->mutex);
}
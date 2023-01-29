#include "btree.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../util/util.h"
#include <math.h>
#include <string.h>
#include <stdbool.h>

BTree *newTree()
{
    BTree *result = malloc(sizeof(BTree));
    result->mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
    result->root = NULL;
    return result;
}

// Retrieves elements from the file in which they are stored and uses them to build the tree
BTree *buildBTreeFromFile(const char *filename)
{
    BTree *tree = newTree();
    FILE *file = fopen(filename, "r");// only read

    if (file != NULL)
    {
        printf("[+]Building tree...\n");
        char *currentLine = NULL;
        size_t strLen = 0;
        char insideBrachet[BUFSIZ];

        while (getline(&currentLine, &strLen, file) != -1) // Scroll through the file line by line
        {
            stringInsideSquareBracket(currentLine, sizeof(insideBrachet), insideBrachet);

            int value = atoi(insideBrachet); // Converts the found string to an integer

            tree->root = insert(tree, value);
        }

        free(currentLine);
        fclose(file);
        printf("[+]Tree built\n");
    }
    else
    {
        printf("[-]Unable to build the tree, the file (%s) does not exist", filename);
    }

    return tree;
}

//Opens storage file (data/data.txt), uses it to build the binary tree, 
//then closes file and returns pointer to the tree.
BTree *buildBTree()
{
    return buildBTreeFromFile("data/data.txt");
}

Node *newNode(int integer)
{
    Node *temp = malloc(sizeof(Node));

    temp->integer = integer;

    temp->left = temp->right = NULL;
    return temp;
}

int getMaxRec(Node *node)
{
    if (node->right == NULL)
        return node->integer;
    return getMaxRec(node->right);
}


int getMax(BTree *tree)
{
    pthread_mutex_lock(&tree->mutex);
    int result = getMaxRec(tree->root);
    pthread_mutex_unlock(&tree->mutex);

    return result;
}

Node *insertRec(Node *node, int data)
{
    /* 1. If the tree is empty, return a new,
     single node */
    if (node == NULL)
        return newNode(data);
    else
    {

        /* 2. Otherwise, recur down the tree */
        if (data < node->integer)
            node->left = insertRec(node->left, data);
        else if (data > node->integer)
            node->right = insertRec(node->right, data);

        /* return the (unchanged) node pointer */
        return node;
    }
}

Node *insert(BTree *tree, int integer)
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

void getAsList(Node *node, List *list)
{

    if (node == NULL)
        return;

    getAsList(node->left, list);
    addToList(node->integer, list);
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

    printf("%d\n", root->integer);

    // Process left child
    print2DUtil(root->left, space);
}


void printTree(BTree *tree)
{
    pthread_mutex_lock(&tree->mutex);
    if (tree->root == NULL)
    {
        printf("[-]Tree is empty\n");
        pthread_mutex_unlock(&tree->mutex);
        return;
    }
    print2DUtil(tree->root, 0);
    pthread_mutex_unlock(&tree->mutex);
}
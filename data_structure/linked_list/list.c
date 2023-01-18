#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "list.h"

list_node *createnode(int *data)
{
    list_node *newNode = malloc(sizeof(list_node));
    if (!newNode)
    {
        return NULL;
    }
    newNode->integer = data;
    newNode->next = NULL;
    return newNode;
}

List *makelist()
{
    List *list = malloc(sizeof(List));
    if (!list)
    {
        return NULL;
    }
    list->head = NULL;
    return list;
}

void displayList(List *list)
{
    list_node *current = list->head;
    if (list->head == NULL)
        return;

    printf("start->");
    for (; current != NULL; current = current->next)
    {
        printf("%d",current->integer);
        printf("-");
    }
    printf("->end\n");
}

void addToList(int *data, List *list)
{
    list_node *current = NULL;
    if (list->head == NULL)
    {
        list->head = createnode(data);
    }
    else
    {
        current = list->head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = createnode(data);
    }
}

void destroyList(List *list)
{
    list_node *current = list->head;
    list_node *next = current;
    while (current != NULL)
    {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}
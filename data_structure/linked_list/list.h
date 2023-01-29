#ifndef LIST_H
#define LIST_H

typedef struct list_node
{
    int integer;
    struct list_node *next;
} list_node;

typedef struct list
{
    list_node *head;
} List;

List *makelist();
void addToList(int data, List *list);
void displayList(List *list);
void destroyList(List *list);


#endif // !LIST_H
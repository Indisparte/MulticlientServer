#ifndef SERVER_H
#define SERVER_H
#include "../data_structure/entity/user.h"
#include "../data_structure/btree/btree.h"
#include <stdbool.h>

enum Command
{
    NEW_INT = 'i', // usage: i [integer]
    LIST = 'l',    // usage: l
    MAX = 'm',     // usage: m
    EXIT = 'e'     // usage: e
};

//Handles functions for each command requested by the client.
//Returns false if a command fails, otherwise true
bool dispatch(User *, int, char *, BTree *);

#endif // !SERVER_H
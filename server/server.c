#include "server.h"
#include "../data_structure/btree/btree.h"
#include "../util/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/socket.h>

// Sends a message on the socket printing an error in case there is.
void sendMsg(const User *user, const char *msg)
{
    if (send(user->client_fd, msg, strlen(msg), 0) == -1)
    {
        perror("[-]Error in sending\n");
        return;
    }
}

bool add_integer(User *user, const char *msg, BTree *tree)
{
    char buffer[2064];
    int to_add;
    buffer[sizeof(buffer)] = '\0';

    stringInsideSquareBracket(msg, sizeof(buffer), buffer);

    to_add = atoi(buffer);

    // Adds a new integer in the tree
    tree->root = insert(tree, to_add);

    // Adds a new integer in the file
    if (!writeOnFile("data/data.txt", to_add))
    {
        printf("[-]Integer not stored in the file!\n");
        return false;
    }
    else
    {
        printf("[+]Store into file successfully!\n");
        return true;
    }
}

bool send_integers(const User *user, BTree *tree)
{
    printf("Sending integers to user\n");
    // get all integers
    List *result = getAllIntegers(tree);
    if (result->head != NULL)
    {
        // build JSON formatted string as response
        char json[50000] = "{\"integers\":["; // json list name
        buildJsonString(result->head, json);

        sendMsg(user, json);

        destroyList(result);

        json[sizeof(json)] = '\0';

        printf("- - - The tree - - -\n");
        printTree(tree);

        return true;
    }
    else
    {
        printf("[-]No integers were found\n");
        return false;
    }
}

void send_max(const User *user, BTree *tree)
{
    char *max_value;
    sprintf(max_value, "%d\n", getMax(tree));
    sendMsg(user, max_value);
    printf("Sending max value (%s) to user\n", max_value);
}

bool dispatch(User *user, int command, char *msg, BTree *tree)
{
    switch (command)
    {
    case NEW_INT:
        printf("User want add an integer\n");
        add_integer(user, msg, tree);
       return true;
    case MAX:
        printf("User want max value\n");
        send_max(user, tree);
        return true;
    case LIST:
        printf("User want list of integers\n");
        if (!send_integers(user, tree))
            printf("[-]Unable to send list of integers\n");
        else
            printf("[+]List of integers correctly send to user\n");
        return true;
    case EXIT:
        printf("User want exit...\n");
        return false;
    default:
        printf("Unknown command (%d) full message: %s\n", command, msg);
        //You can send an error message to alert the client (like 404)
        //return false;
    }
    return true;
}
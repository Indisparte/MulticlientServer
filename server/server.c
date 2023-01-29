#include "server.h"
#include "../data_structure/btree/btree.h"
#include "../util/util.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <sys/socket.h>


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
    int to_add ;
    buffer[sizeof(buffer)] = '\0';

    stringInsideSquareBracket(msg, sizeof(buffer), buffer);

printf("In bracket %s",buffer);
    to_add = atoi(buffer);

    // add new pothole into tree
    tree->root = insert(tree, to_add);

    // add new pothole into file
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
        // build JSON as response
        char json[50000] = "{\"integers\":[";
        buildJsonString(result->head, json);
        sendMsg(user, json);

        destroyList(result);

        json[sizeof(json)] = '\0';
        return true;
    }
    else
    {
        printf("[-]No integers were found\n");
        return false;
    }
    printf("- - - The tree - - -\n");
    printTree(tree);
}

void send_max(const User *user, BTree *tree)
{
    char* max_value;
    sprintf(max_value, "%d\n", getMax(tree));
    sendMsg(user, max_value);
    printf("Sending max to user\n");
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
            printf("[-] Unable to send list of integers\n");
        else
            printf("[+] List of integers correctly send to user\n");
        return true;
    case EXIT:
        printf("User want exit...\n");
        return false;
    default:
        printf("Unknown command (%d) full message: %s\n", command, msg);
        return false;
    }
    return true;
}
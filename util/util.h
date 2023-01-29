#ifndef UTIL_H
#define UTIL_H

#include "../data_structure/linked_list/list.h"
#include <stdbool.h>
#include <assert.h>

/*
Gets the string between the square brackets 'in', and stores it in the string 'out'.
 Return length of substring.
 Return -1 when not found or out[] too small.
*/
long stringInsideSquareBracket(const char *in, long out_size, char *out);

//Writes a value to the specified file, 
//if the file does not exist provides for its creation
bool writeOnFile(const char *filename, int integer);

/*
Build and return a json string from a list of integers.
{integers:[{<integer>,<integer>,...,<integer>}]}
*/
void buildJsonString(const list_node *node, char *to_send);


#endif // !UTIL_H
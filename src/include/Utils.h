#ifndef UTILS_H
#define UTILS_H

#include <string.h>

/** 
 * @breif Compares the values of 2 strings
 *
 *
 * @return strcmp of the 2 params
 * */
int cmp_string(const void* a, const void* b);
/** 
 * @breif Runs a binary search on a sorted array of strings
 *
 * @return Returns the index of string in array or -1 if not found
 * */
int sorted_string_search(char** arr, int size, char* value); 

#endif

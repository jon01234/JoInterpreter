#include "include/Utils.h"

int cmp_string(const void* a, const void* b)
{
    const char* aa = *(const char**)a;
    const char* bb = *(const char**)b;

    return strcmp(aa, bb);
}

int sorted_string_search(char** arr, int size, char* value)
{
    int low = 0;
    int high = size - 1;
    int mid;
    int comparison;

    while (low <= high) {
        mid = low + (high - low) / 2;
        comparison = strcmp(arr[mid], value);
        if (comparison == 0) {
            return mid;
        } else if (comparison < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    return -1;
}

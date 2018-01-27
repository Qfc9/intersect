#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "util.h"

// Sorting a word by their value
int
sortByAscii(
    char *str1,
    char *str2)
{
    // Getting the length of the shortest word
    size_t          shortestSz = strlen(str1);

    if (shortestSz > strlen(str2))
    {
        shortestSz = strlen(str2);
    }

    // Looping the comparing each char
    for (size_t i = 0; i < shortestSz; i++)
    {
        if (str1[i] > str2[i])
        {
            return 1;
        }
        else if (str1[i] < str2[i])
        {
            return -1;
        }
    }

    // Check if the first word is longer
    if (strlen(str1) < strlen(str2))
    {
        return -1;
    }
    // Check if the second word is longer
    else if (strlen(str1) > strlen(str2))
    {
        return 1;
    }

    // It is a match
    return 0;
}

// Returns a lowercase version of a word 
char *
stringToLower(
    char *word)
{
    // Mallocing new empty word
    char           *lowWord = malloc(sizeof(*lowWord) * (strlen(word) + 1));

    if (!lowWord)
    {
        return NULL;
    }

    // Copying word over but the lowercase version
    for (size_t i = 0; i < strlen(word); i++)
    {
        lowWord[i] = tolower(word[i]);
    }

    // Null terminating
    lowWord[strlen(word)] = '\0';

    return lowWord;
}

#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "util.h"

int sortByAscii(char *str1, char *str2)
{
    size_t shortestSz = strlen(str1);
    if (shortestSz > strlen(str2))
    {
        shortestSz = strlen(str2);
    }

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

    if (strlen(str1) < strlen(str2))
    {
        return -1;
    }
    else if (strlen(str1) > strlen(str2))
    {
        return 1;
    }

    return 0;
}

size_t wordValue(char *word)
{
    size_t value = 0;

    for (size_t i = 0; i < strlen(word); i++)
    {
        //value += tolower(word[i]);
        value += word[i];
    }

    return value;
}

char * stringToLower(char *word)
{
    char *lowWord = malloc(sizeof(*lowWord) * (strlen(word) + 1));
    if(!lowWord)
    {
        return NULL;
    }

    for (size_t i = 0; i < strlen(word); i++)
    {
        lowWord[i] = tolower(word[i]);
    }

    lowWord[strlen(word)] = '\0';

    return lowWord;
}

// FIX FOR UTF-8
void stringToLowerVoid(char *word)
{
    for (size_t i = 0; i < strlen(word); i++)
    {
        word[i] = tolower(word[i]);
    }
    word[strlen(word)] = '\0';
}


// Removes extra chars at the end of stdin
void
getCharClean (void)
{
    int clearChar = getchar ();
    while ((clearChar != '\n') && (clearChar != EOF))
    {
        clearChar = getchar ();
    }
}

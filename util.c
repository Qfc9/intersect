#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "util.h"

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

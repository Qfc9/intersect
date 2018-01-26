#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "util.h"

size_t wordValue(char *word)
{

  return 0;
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

#include <stdio.h>
#include <stdlib.h>

#include "readIn.h"

void getData(FILE *fp, char ***data, size_t *sz, size_t *cap)
{
    int tempStorageMax = 64;
    int storageCounter = 0;

    *data = calloc (1, *cap * sizeof (**data));

    // Getting all the data in the file given
    char buf = fgetc (fp);
    while (buf != EOF)
    {
        char *storage = malloc(tempStorageMax);

        // Dynamicly allocating each line in the file as a string
        while (buf != EOF && buf != 10 && buf != 35)
        {

            storage[storageCounter] = buf;

            storageCounter++;
            buf = fgetc (fp);

            // Allocating more memory for the line
            if (storageCounter >= tempStorageMax - 1)
            {
                tempStorageMax *= 2;
                void *tmp = realloc (storage, tempStorageMax * sizeof (storage));
                if (tmp)
                {
                    storage = tmp;
                }
                else
                {
                    // Freeing mallocs
                    for (unsigned int n = 0; n < *sz; n++)
                    {
                        free (*data[n]);
                    }
                    free (*data);
                    return;
                }
            }
        }

        // Adding eaching line to an array
        storage[storageCounter] = '\0';
        buf = fgetc (fp);
        storageCounter = 0;
        (*data)[(*sz)++] = storage;
        // Allocating more memory for the array for lines
        if ((*sz) >= (*cap))
        {
            (*cap) *= 2;
            void *tmp = realloc (*data, *cap * sizeof (**data));
            if (tmp)
            {
                *data = tmp;
            }
            else
            {
                // Freeing mallocs
                for (unsigned int n = 0; n < *sz; n++)
                {
                    free (*data[n]);
                }
                free (*data);
                return;
            }
        }
    }

}
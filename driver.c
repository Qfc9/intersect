#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readIn.h"
#include "tree.h"

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        fprintf(stderr, "Invalid amount of args\n");
        return 1;
    }

    FILE *fp = fopen (argv[1], "r");
    size_t fileDataSz = 0;
    size_t fileDataCap = 4;

    char **fileData;

    if (fp == NULL)
    {
        fprintf (stderr, "Unable to open the file: %s\n", argv[1]);
        return 1;
    }

    getData(fp, &fileData, &fileDataSz, &fileDataCap);

    fclose(fp);

    // Creating the intersectTree tree
    tree *intersectTree = createTree ();

    // Adding all the values from the file into the tree
    for (unsigned int n = 0; n < fileDataSz; n++)
    {
        processLine (&intersectTree, fileData[n]);
    }

    for (int i = 2; i < argc; ++i)
    {
        fp = fopen(argv[i], "r");
        if(!fp)
        {
            continue;
        }

        treeIntersects(&intersectTree, fp, i);

        fclose(fp);
    }

    treePrint (intersectTree);
    printf ("\n");

    // Freeing input mallocs
    for (unsigned int n = 0; n < fileDataSz; n++)
    {
        free (fileData[n]);
    }
    free (fileData);

    // Freeing the tree
    treeDisassemble (intersectTree);

    return 0;
}

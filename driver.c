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

    FILE *fp;
    size_t fileDataSz = 0;
    size_t fileDataCap = 4;

    char **fileData;

    if (strcmp(argv[1], "-") != 0)
    {
        fp = fopen (argv[1], "r");
        if (fp == NULL)
        {
            fprintf (stderr, "Unable to open the file: %s\n", argv[1]);
            return 1;
        }

        getData(fp, &fileData, &fileDataSz, &fileDataCap);

        fclose(fp);
    }
    else
    {
        getData(stdin, &fileData, &fileDataSz, &fileDataCap);
        printf ("\n");
    }

    // Creating the intersectTree tree
    tree *intersectTree = createTree ();

    // Adding all the values from the file into the tree
    for (unsigned int n = 0; n < fileDataSz; n++)
    {
        processLine (&intersectTree, fileData[n]);
        free (fileData[n]);
    }
    free (fileData);


    size_t skippedFiles = 0;
    for (int i = 2; i < argc; ++i)
    {
        fp = fopen(argv[i], "r");
        if(!fp)
        {
            fprintf (stderr, "Unable to open the file: %s\n", argv[i]);
            skippedFiles += 1;
            continue;
        }
        treeIntersects(&intersectTree, fp, (i - skippedFiles));

        fclose(fp);
    }

    treePrint (intersectTree);
    printf ("\n");

    // Freeing the tree
    treeDisassemble (intersectTree);

    return 0;
}

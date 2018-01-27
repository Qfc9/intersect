#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "readIn.h"
#include "tree.h"

// This is the program's main function
int
main(
    int argc,
    char *argv[])
{
    // Checking for at least 2 args
    if (argc <= 2)
    {
        fprintf(stderr, "Invalid amount of args\n");
        return 1;
    }

    // Initializing variables for reading in the first file
    FILE           *fp;
    size_t          fileDataSz = 0;
    size_t          fileDataCap = 4;
    char          **fileData;

    // Checking if the first arg not a -
    if (strcmp(argv[1], "-") != 0)
    {
        // Open the first file and check if it's legit
        fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            fprintf(stderr, "Unable to open the file: %s\n", argv[1]);
            return 1;
        }

        // Reading the file and storing everything in the file data variable
        getData(fp, &fileData, &fileDataSz, &fileDataCap);

        fclose(fp);
    }
    // If the first arg is a - read from STDIN
    else
    {
        getData(stdin, &fileData, &fileDataSz, &fileDataCap);
        printf("\n");
    }

    // Creating the intersectTree tree
    tree           *intersectTree = createTree();

    // Adding all the values from the first file into the tree
    for (unsigned int n = 0; n < fileDataSz; n++)
    {
        treeAddWords(&intersectTree, fileData[n]);
        free(fileData[n]);
    }
    free(fileData);


    // Reading every file after the first one
    size_t          skippedFiles = 0;

    for (int i = 2; i < argc; ++i)
    {
        // Making sure the file opens
        fp = fopen(argv[i], "r");
        if (!fp)
        {
            // Go to the next file if it doesn't open
            fprintf(stderr, "Unable to open the file: %s\n", argv[i]);
            skippedFiles += 1;
            continue;
        }

        // Do an intersection against the current file
        treeIntersects(&intersectTree, fp, (i - skippedFiles));

        fclose(fp);
    }

    // Printing the tree
    treePrint(intersectTree);
    printf("\n");

    // Free the tree
    treeDisassemble(intersectTree);

    return 0;
}

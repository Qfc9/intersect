#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        fprintf(stderr, "Invalid amount of args\n");
        return 1;
    }
    
    return 0;
}
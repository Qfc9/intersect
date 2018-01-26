#ifndef tree_H
#define tree_H

#include <stdlib.h>

typedef struct _tree tree;
tree *createTree (void);
void processCompany (tree ** t, char *line);
size_t treeHeight (tree * a);
void treeInsert (tree ** a, char *word, size_t price);
void treePrint (const tree * a);
void treeDisassemble (tree * a);

#endif

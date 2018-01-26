#ifndef tree_H
#define tree_H

#include <stdlib.h>

typedef struct _tree tree;
tree *createTree (void);
void processCompany (tree ** t, char *line);
size_t treeHeight (tree * a);
void treeUpdate (tree ** t, char *ticker, char *name, double value);
void treeInsert (tree ** a, char *symbol, char *name, size_t price);
void treePrint (const tree * a);
void treeDisassemble (tree * a);

#endif

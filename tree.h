#ifndef tree_H
#define tree_H

#include <stdlib.h>

typedef struct _tree tree;
void processLine (tree ** t, char *line);
tree *createTree (void);
size_t treeHeight (tree * a);
void treeInsert (tree ** t, char *word, size_t index);
void treePrint (const tree * a);
void treeDisassemble (tree * a);

#endif

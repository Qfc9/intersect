#ifndef tree_H
#define tree_H

#include <stdlib.h>

typedef struct _tree tree;
void
treeAddWords(
    tree ** t,
    char *line);
tree *
createTree(
    void);
size_t
treeHeight(
    tree * t);
void
treeInsert(
    tree ** t,
    char *word,
    char *lowWord,
    size_t value);
void
treePrint(
    const tree * a);
void
treeDisassemble(
    tree * a);
void
treeIntersects(
    tree ** t,
    FILE * fp,
    size_t index);

#endif

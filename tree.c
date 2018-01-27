#define _XOPEN_SOURCE 600

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "util.h"
#include "tree.h"

// node Storage
struct node
{
    size_t index;
    char *lowWord;
    char *word;
};

// Hidden tree struct
struct _tree
{
    struct node *data;
    struct _tree *left;
    struct _tree *right;
};


// Initializing static functions
static void _rebalance (struct _tree **a);
static void _rotateLeft (struct _tree **a);
static void _rotateRight (struct _tree **a);
static void _treeMarkIntersects(tree ** t, char *word);
static void _removeTreeIntersects(struct _tree **t, size_t index);
static struct _tree * _treeFind(struct _tree *t, char *word);
static void _treeRemove (tree ** a, size_t value);
static struct node *_treeCreateData (char *word,
              size_t value);

// Creating a new tree
tree *
createTree (void)
{
    return NULL;
}

void processLine (tree ** t, char *line)
{
    if (!t)
    {
        return;
    }

    char buf[255];
    char *lowBuf = NULL;

    strncpy(buf, "\0", 255);

    int tracker = 0;
    int tempTracker = 0;

    // Continueing from the last marked spot, getting the company name
    while (sscanf (line + tracker, "%254s%n ", buf, &tempTracker) > 0)
    {
        lowBuf = stringToLower(buf);
        treeInsert (t, buf, lowBuf, 1);
        tracker += tempTracker;
        free(lowBuf);
    }
}

void treeIntersects(tree ** t, FILE *fp, size_t index)
{
    if (!t || !*t || !fp)
    {
        return;
    }

    int wordSz = 0;
    char *word = NULL;

    // Getting all the data in the file given
    char buf = 0;
    while (buf != EOF)
    {
        buf = fgetc (fp);
        wordSz = 0;
        word = malloc(sizeof(*word) * 255);
        if(!word)
        {
            break;
        }

        // Dynamicly allocating each line in the file as a string
        while (buf != EOF && buf != 10 && buf != 32)
        {
            word[wordSz++] = buf;
            buf = fgetc (fp);
        }
        word[wordSz] = '\0';

        if(wordSz > 0)
        {
            char *lowWord = stringToLower(word);
            if(!lowWord)
            {
                free(word);
                return;
            }
            _treeMarkIntersects(t, lowWord);
            free(lowWord);
        }

        free(word);
    }
    _removeTreeIntersects(t, index - 1);
    _removeTreeIntersects(t, index - 1);
}

// Geting the height of a tree
size_t
treeHeight (tree * a)
{
    if (!a)
    {
        return 0;
    }
    size_t left = treeHeight (a->left);
    size_t right = treeHeight (a->right);

    return 1 + (left > right ? left : right);
}

// Inserting a new node into a tree
void
treeInsert (tree ** t, char *word, char *lowWord, size_t value)
{
    if (!t || !lowWord)
    {
        return;
    }

    // If the tree is null make a root
    if (!*t)
    {
        *t = calloc (1, sizeof (**t));

        // Checking if calloc properly
        if (!*t)
        {
            return;
        }

        // Making new node 
        struct node *newStock = _treeCreateData (word, value);
        // Checking if malloced properly
        if (!newStock)
        {
            free (*t);
            return;
        }
        (*t)->data = newStock;

        return;
    }

    tree *subTree = *t;

    int cmpVal = sortByAscii(lowWord, subTree->data->lowWord);
    // Inserting the node in the correct spot on the tree
    if (cmpVal < 0)
    {
        treeInsert (&subTree->left, word, lowWord, value);
    }
    else if(cmpVal == 0)
    {
        return;
    }
    else
    {
        treeInsert (&subTree->right, word, lowWord, value);
    }

    // Rebalancing the tree
    _rebalance (t);
}

// Printing all the values on the tree
void
treePrint (const tree * a)
{
    if (!a)
    {
        return;
    }
    treePrint (a->left);
    printf ("%s\n", a->data->word);
    treePrint (a->right);
}

// Freeing all members on the tree
void
treeDisassemble (tree * a)
{
    if (!a)
    {
        return;
    }

    treeDisassemble (a->left);
    treeDisassemble (a->right);
    free (a->data->word);
    free (a->data->lowWord);
    free (a->data);
    free (a);
}

static void _removeTreeIntersects(struct _tree **t, size_t index)
{
    if (!t)
    {
        return;
    }
    else if (!*t)
    {
        return;
    }

    _removeTreeIntersects (&(*t)->left, index);
    _removeTreeIntersects (&(*t)->right, index);

    _treeRemove(t, index);
}

static void _treeMarkIntersects(tree ** t, char *word)
{
    if (!t || !*t)
    {
        return;
    }

    tree *foundTree = _treeFind(*t, word);
    if(foundTree)
    {
        foundTree->data->index += 1;
    }
}

static struct _tree * _treeFind(struct _tree *t, char *word)
{
    if (!t)
    {
        return NULL;
    }

    int cmpVal = sortByAscii(word, t->data->lowWord);

    // Inserting the node in the correct spot on the tree
    if (cmpVal < 0)
    {
        return _treeFind (t->left, word);
    }
    else if(cmpVal == 0)
    {
        return t;
    }
    else
    {
        return _treeFind (t->right, word);
    }

    return NULL;
}

tree **get_max(tree **node)
{
    if((*node)->right) {
        return get_max(&(*node)->right);
    } else {
        return node;
    }
}

// Removed Item from tree
static void
_treeRemove (tree ** a, size_t value)
{
    if (!a || !*a)
    {
        return;
    }

    struct _tree *t = *a;

    // If the symbol and money values are the same, Its a match
    if ((t->data->index <= value))
    {
        // If no children
        if (!t->left && !t->right)
        {
            free (t->data->word);
            free (t->data->lowWord);
            free (t->data);
            free (t);

            *a = NULL;
        }
        // If one child
        else if (!t->left || !t->right)
        {
            if (t->left)
            {
                *a = t->left;
            }
            else
            {
                *a = t->right;
            }
            free (t->data->word);
            free (t->data->lowWord);
            free (t->data);
            free (t);
        }
        // If two children
        else
        {
            // Get the greatest value 
            tree **newValue = get_max(&(t)->left);
            free (t->data->word);
            free (t->data->lowWord);
            free (t->data);

            // Move greatest value to old stock position
            struct node *newStock = _treeCreateData ((*newValue)->data->word, (*newValue)->data->index);
            t->data = newStock;
            (*newValue)->data->index = 0;

            // Remove old stock and rebalance 
            _treeRemove (&(t)->left, value);
        }
    }

    _rebalance (a);
}

// Returns a node * with all the information set
static struct node *
_treeCreateData (char *word, size_t value)
{
    // Mallocing 
    struct node *newStock = malloc (sizeof (*newStock));
    // Checking malloc
    if (!newStock)
    {
        return NULL;
    }

    // Setting word
    newStock->word = strdup (word);
    // Checking
    if (!newStock->word)
    {
        free (newStock);
        return NULL;
    }

    // Setting word
    newStock->lowWord = stringToLower(word);
    // Checking
    if (!newStock->lowWord)
    {
        free (newStock->word);
        free (newStock);
        return NULL;
    }

    // Setting value
    newStock->index = value;

    return newStock;
}

// Rebalancing the tree, SAME FROM IN CLASS
static void
_rebalance (struct _tree **a)
{
    if (!a || !*a)
    {
        return;
    }

    struct _tree *t = *a;
    // If tree needs rebalancing, do so
    size_t left_height = treeHeight (t->left);
    size_t right_height = treeHeight (t->right);

    if (left_height > right_height && left_height - right_height > 1)
    {
        size_t left_left_height = treeHeight (t->left->left);
        size_t left_right_height = treeHeight (t->left->right);
        if (left_left_height < left_right_height)
        {
            // First rotate child to the left
            _rotateLeft (&t->left);
        }
        // Rotate right
        _rotateRight (a);
    }
    else if (left_height < right_height && right_height - left_height > 1)
    {
        size_t right_right_height = treeHeight (t->right->right);
        size_t right_left_height = treeHeight (t->right->left);
        if (right_right_height < right_left_height)
        {
            // First rotate child to the right
            _rotateRight (&t->right);
        }
        // Rotate left
        _rotateLeft (a);
    }
}

// Helps Rebalance, rotating tree right
static void
_rotateRight (struct _tree **t)
{
    if (!t)
    {
        return;
    }
    else if (!*t)
    {
        return;
    }
    struct _tree *child = (*t)->left;
    (*t)->left = child->right;
    child->right = *t;
    *t = child;
}

// Helps Rebalance, rotating tree right
static void
_rotateLeft (struct _tree **t)
{
    if (!t)
    {
        return;
    }
    else if (!*t)
    {
        return;
    }
    struct _tree *child = (*t)->right;
    (*t)->right = child->left;
    child->left = *t;
    *t = child;
}
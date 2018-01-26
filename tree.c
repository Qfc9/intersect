#define _XOPEN_SOURCE 600

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "tree.h"

// node Storage
struct node
{
    size_t index;
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
static void treeRemove (tree ** a, size_t value);
static struct _tree *treeGetMax (struct _tree *t);
static struct node *treeCreateStock (char *word,
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

    int tracker = 0;
    int tempTracker = 0;

    // Continueing from the last marked spot, getting the company name
    while (sscanf (line + tracker, "%254s%n ", buf, &tempTracker) > 0)
    {
        treeInsert (t, buf, wordValue(buf));
        tracker += tempTracker;
    }
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
treeInsert (tree ** t, char *word, size_t value)
{
    if (!t)
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
        struct node *newStock = treeCreateStock (word, value);
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

    // Inserting the node in the correct spot on the tree
    if (value <= subTree->data->index)
    {
        treeInsert (&subTree->left, word, value);
    }
    else
    {
        treeInsert (&subTree->right, word, value);
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
    printf ("%zu.", (a->data->index / 100));
    printf ("%02zu ", (a->data->index % 100));
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
    free (a->data);
    free (a);
}

// Removed Item from tree
static void
treeRemove (tree ** a, size_t value)
{
    if (!a)
    {
        return;
    }
    if (!*a)
    {
        return;
    }

    struct _tree *t = *a;

    // If the symbol and money values are the same, Its a match
    if ((t->data->index == value))
    {
        // If no children
        if (!t->left && !t->right)
        {
            free (t->data->word);
            free (t->data);
            free (t);

            *a = NULL;
            return;
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
            free (t->data);
            free (t);
        }
        // If two children
        else
        {
            // Get the greatest value 
            tree *newValue = treeGetMax (t->left);
            free (t->data->word);
            free (t->data);

            // Move greatest value to old stock position
            struct node *newStock = treeCreateStock (newValue->data->word, newValue->data->index);
            t->data = newStock;

            // Remove old stock and rebalance 
            treeRemove (&t->left, newValue->data->index);
            _rebalance (a);
        }

    }
    // Keep Searching
    else if (t->data->index < value)
    {
        treeRemove (&t->right, value);
    }
    else
    {
        treeRemove (&t->left, value);
    }
}

// Get Highest value on the tree
static struct _tree *
treeGetMax (struct _tree *t)
{
    if (t->right)
    {
        return treeGetMax (t->right);
    }
    else
    {
        return t;
    }
}

// Returns a node * with all the information set
static struct node *
treeCreateStock (char *word, size_t value)
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

    // Setting value
    newStock->index = value;

    return newStock;
}

// Rebalancing the tree, SAME FROM IN CLASS
static void
_rebalance (struct _tree **a)
{
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
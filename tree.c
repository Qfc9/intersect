#define _XOPEN_SOURCE 600

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "tree.h"

// Company Storage
struct company
{
  char symbol[6];
  size_t cents;
  char *name;
};

// Hidden tree struct
struct _tree
{
  struct company *data;
  struct _tree *left;
  struct _tree *right;
};


// Initializing static functions
static void _rebalance (struct _tree **a);
static void _rotateLeft (struct _tree **a);
static void _rotateRight (struct _tree **a);
static void treeRemove (tree ** a, char *ticker, size_t value);
static struct _tree *treeGetMax (struct _tree *t);
static struct _tree *treeSearchForName (struct _tree *t, char *name);
static struct company *treeCreateStock (char *symbol, char *name,
                    size_t price);

// Creating a new tree
tree *
createTree (void)
{
  return NULL;
}

// Extracts company info from a string
void
processCompany (tree ** t, char *line)
{
  if (!t)
    {
      return;
    }

  double value = 0.0;
  char ticker[6];
  char name[64];
  char buf[64];

  int tracker = 0;
  int tempTracker = 0;

  strcpy (name, "\0");
  strcpy (buf, "\0");
  strcpy (ticker, "\0");


  // Getting ticker and money, tracking where it left off
  if (sscanf (line, "%5s %lf%n", ticker, &value, &tracker) != 2)
    {
      return;
    }

  // Verifing the ticker
  // if (invalidTicker (ticker))
  //   {
  //     return;
  //   }

  // Continueing from the last marked spot, getting the company name
  while (sscanf (line + tracker, "%63s%n", buf, &tempTracker) > 0)
    {
      // Making sure the name isn't to long
      if ((strlen (buf) + strlen (name)) <= 63)
    {
      strcat (name, buf);
      strcat (name, " ");
      tracker += tempTracker;

    }
      else
    {
      strncat (name, buf, 63 - strlen (name));
      name[63] = '\0';
      break;
    }
    }

  // Updating the tree with this new info
  treeUpdate (t, ticker, name, value);
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

// Inserting a new company into a tree
void
treeInsert (tree ** a, char *symbol, char *name, size_t price)
{
  if (!a)
    {
      return;
    }

  // If the tree is null make a root
  if (!*a)
    {
      *a = calloc (1, sizeof (**a));

      // Checking if calloc properly
      if (!*a)
    {
      return;
    }

      // Making new company 
      struct company *newStock = treeCreateStock (symbol, name, price);
      // Checking if malloced properly
      if (!newStock)
    {
      free (*a);
      return;
    }
      (*a)->data = newStock;
      return;
    }

  tree *t = *a;

  // Inserting the company in the correct spot on the tree
  if (price <= t->data->cents)
    {
      treeInsert (&t->left, symbol, name, price);
    }
  else
    {
      treeInsert (&t->right, symbol, name, price);
    }

  // Rebalancing the tree
  _rebalance (a);
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
  printf ("%s ", a->data->symbol);
  printf ("%zu.", (a->data->cents / 100));
  printf ("%02zu ", (a->data->cents % 100));
  printf ("%s\n", a->data->name);
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
  free (a->data->name);
  free (a->data);
  free (a);
}

// Updating the tree 
void
treeUpdate (tree ** t, char *ticker, char *name, double value)
{
  if (!t)
    {
      return;
    }

  // Searching the tree for a ticker
  tree *result = treeSearchForName (*t, ticker);

  size_t covertedVal = 0;
  size_t newValue = 0;

  // If that ticker doesn't exsist add it
  if (!result)
    {
      treeInsert (t, ticker, name, covertedVal);
      return;
    }

  // Subing the new and old stock
  if (value < 0 && covertedVal <= result->data->cents)
    {
      newValue = result->data->cents - covertedVal;
    }
  // Adding the new and old stock
  else if ((covertedVal + result->data->cents) <= 100000000)
    {
      newValue = result->data->cents + covertedVal;
    }
  // Invalid stock information
  else
    {
      return;
    }

  // Coping stock info, originals wont last the remove
  char newTick[6];
  char newName[64];
  strncpy (newTick, result->data->symbol, 5);
  strncpy (newName, result->data->name, 63);
  newTick[5] = '\0';
  newName[63] = '\0';

  // Removing old stock and added the new one
  treeRemove (t, ticker, result->data->cents);
  treeInsert (t, newTick, newName, newValue);
}

// Removed Item from tree
static void
treeRemove (tree ** a, char *ticker, size_t value)
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
  if ((t->data->cents == value) && (strcmp (t->data->symbol, ticker) == 0))
    {
      // If no children
      if (!t->left && !t->right)
    {
      free (t->data->name);
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
      free (t->data->name);
      free (t->data);
      free (t);
    }
      // If two children
      else
    {
      // Get the greatest value 
      tree *newValue = treeGetMax (t->left);
      free (t->data->name);
      free (t->data);

      // Move greatest value to old stock position
      struct company *newStock =
        treeCreateStock (newValue->data->symbol, newValue->data->name,
                 newValue->data->cents);
      t->data = newStock;

      // Remove old stock and rebalance 
      treeRemove (&t->left, newValue->data->symbol,
              newValue->data->cents);
      _rebalance (a);
    }

    }
  // Keep Searching
  else if (t->data->cents < value)
    {
      treeRemove (&t->right, ticker, value);
    }
  else
    {
      treeRemove (&t->left, ticker, value);
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

// Search the tree for a Ticker name, stepping through inorder
static struct _tree *
treeSearchForName (struct _tree *t, char *name)
{
  if (!t)
    {
      return NULL;
    }

  struct _tree *result = NULL;

  // If tickers are the same
  if (strcmp (name, t->data->symbol) == 0)
    {
      result = t;
    }
  // If they aren't the same keep searching
  else
    {
      result = treeSearchForName (t->left, name);
      if (!result)
    {
      result = treeSearchForName (t->right, name);
    }
    }

  return result;
}

// Returns a company * with all the information set
static struct company *
treeCreateStock (char *symbol, char *name, size_t price)
{
  // Mallocing 
  struct company *newStock = malloc (sizeof (*newStock));
  // Checking malloc
  if (!newStock)
    {
      return NULL;
    }

  // Setting name
  newStock->name = strdup (name);
  // Checking
  if (!newStock->name)
    {
      free (newStock);
      return NULL;
    }

  // Setting symbol
  strncpy (newStock->symbol, symbol, sizeof (newStock->symbol) - 1);
  newStock->symbol[sizeof (newStock->symbol) - 1] = '\0';

  // Setting price
  newStock->cents = price;

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

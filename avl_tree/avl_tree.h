#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>


struct Node 
{
    int key = 0;
    unsigned char height = 0;
    struct Node *left = nullptr;
    struct Node *right = nullptr;
};

struct Avl_tree
{
    struct Node *root = nullptr;
};

struct Avl_tree *avl_tree_ctor ();
void insert (struct Avl_tree *avl_tree, int key);
void Remove (struct Avl_tree *tree, int remove_key);
void avl_tree_dtor (struct Avl_tree *avl_tree);

struct Node *node_ctor (int key);
int node_height (struct Node *node);
int b_factor (struct Node *node);
void fix_height (struct Node *node);
inline unsigned char max (unsigned char val1, unsigned char val2);
struct Node *left_rotate (struct Node *root);
struct Node *right_rotate (struct Node *root);
struct Node *balance_tree (struct Node *root);
struct Node *insert_node (struct Node *root, int key);
struct Node *find_min (struct Node *root);
struct Node *remove (struct Node *root, int remove_key);
struct Node *remove_min (struct Node *root);
void clear_node (struct Node *node);
#endif /* AVL_TREE_H */
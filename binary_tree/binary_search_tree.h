#ifndef BINARY_SEARCH_TREE_H
#define BINARY_SEARCH_TREE_H

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef int Tkey;


struct Node 
{
    Tkey key;
    Node *left = nullptr;
    Node *right = nullptr;
};

struct Binary_tree
{
    Node *root = nullptr;
    size_t size = 0;
};

Binary_tree *bin_tree_ctor ();
Node *bin_tree_find (Node *node, Tkey key);
Node *bin_tree_min (Node *node);
Node *bin_tree_max (Node *node);
Node *bin_tree_next (Binary_tree *tree, Tkey key);
Node *bin_tree_prev (Binary_tree *tree, Tkey key);
void bin_tree_insert (Binary_tree *tree, Tkey key);
Node *bin_tree_insert_node (Binary_tree *tree, Node *node, Tkey key);
void bin_tree_remove (Binary_tree *tree, Tkey key);
Node *bin_tree_delete_node (Binary_tree *tree, Node *node, Tkey key);
void bin_tree_dtor (Binary_tree *tree);
void bin_tree_clear (Node *node);

#endif /* BINARY_SEARCH_TREE_H */
#include <assert.h>
#include <stdio.h>

#include "binary_search_tree.h"

int main ()
{
    Binary_tree *tree = bin_tree_ctor ();

    for (int i = 0; i < 1000; ++i)
    {
        bin_tree_insert (tree, i);
    }
    printf ("%d\n\n", tree->root->key);
    printf ("%p\n\n", tree->root->left);
    for (int i = 0; i < 100; ++i)
    {
        Node *node = bin_tree_find (tree->root, i);
        
        if (node)
            printf ("%d\n", node->key);

    }
    printf ("%d\n", bin_tree_min(tree->root)->key);
    printf ("%d\n", bin_tree_max(tree->root)->key);
    
    for (int i = 0; i < 1000; ++i)
    {
        bin_tree_remove (tree, i);
    }

    printf ("%lu", tree->size);

    bin_tree_dtor (tree);

    return 0;
}
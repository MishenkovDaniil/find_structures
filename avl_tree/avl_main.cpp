#include <assert.h>
#include <stdio.h>

#include "avl_tree.h"

int main ()
{
    struct Avl_tree *avl_tree = avl_tree_ctor ();
    assert (avl_tree);

    for (int i = 0; i < 1000; ++i)
    {
        insert (avl_tree, i);
    }

    for (int i = 0; i < 500; ++i)
    {
        avl_tree->root = remove (avl_tree->root, i);
    }
    
    if (avl_tree->root)
    {
        printf ("%d", avl_tree->root->height);
    }
    avl_tree_dtor (avl_tree);

    return 0;
}
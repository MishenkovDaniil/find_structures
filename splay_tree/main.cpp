#include <stdio.h>
#include <stdlib.h>

#include "splay_tree.h"

int main ()
{
    Splay_tree *splay_tree = splay_ctor ();

    for (int i = 0; i < 20; ++i)
    {
        Insert (splay_tree, i);
        // printf ("%d\n", splay_tree->root->key);
    }
    // splay_dump (splay_tree);

    for (int i = 0; i < 10; ++i)
    {
        find (splay_tree, i);
        // splay_dump (splay_tree);
        // printf ("%d\n", splay_tree->root->key);
    }
    for (int i = 0; i < 5; ++i)
    {
        Remove (splay_tree, i);
        splay_dump (splay_tree);
    }

    splay_dtor (splay_tree);
}
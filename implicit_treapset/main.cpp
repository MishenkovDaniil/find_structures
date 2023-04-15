#include <stdio.h>
#include <assert.h>

#include "implicit_treapset.h"

int main ()
{
    size_t num = 0;
    assert (scanf ("%lu", &num));

    Implicit_treapset *treapset = implicit_treapset_ctor ();

    for (size_t i = 0; i < num; ++i)
    {
        int pos = 0;
        int key = 0;  
        assert (scanf ("%d %d", &key, &pos));
        Insert (treapset, key, pos);
    }

    treapset_dump (treapset);
    Treapset_dtor (treapset);
    return 0;
}
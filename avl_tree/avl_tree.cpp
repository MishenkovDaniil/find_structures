#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "avl_tree.h"

struct Avl_tree *avl_tree_ctor ()
{
    struct Avl_tree *avl_tree = (struct Avl_tree *)calloc (1, sizeof (struct Avl_tree));
    assert (avl_tree);

    avl_tree->root = nullptr;

    return avl_tree;
}

struct Node *node_ctor (int key)
{
    struct Node *node = (struct Node *)calloc (1, sizeof (struct Node));
    assert (node);

    node->key = key;
    node->height = 1;
    node->left = node->right = nullptr;

    return node;
}

void insert (struct Avl_tree *avl_tree, int key)
{
    assert (avl_tree);

    avl_tree->root = insert_node (avl_tree->root, key);
}

struct Node *insert_node (struct Node *root, int key)
{
    if (!(root))
    {
        return node_ctor (key);
    }

    if (key < root->key)
    {
        root->left = insert_node (root->left, key);
    }
    else
    {
        root->right = insert_node (root->right, key);
    }

    return balance_tree (root);
}   

struct Node *remove (struct Node *root, int remove_key)
{
    if (!(root))
    {
        return nullptr;
    }

    if (root->key == remove_key)
    {
        if (root->right)
        {
            root->key = (find_min (root->right))->key; 

            root->right = remove_min (root->right);    
        }
        else
        {
            struct Node *old_root = root;
            root = root->left;
            
            free (old_root);
        }
    }
    else if (remove_key > root->key)
    {
        root->right = remove (root->right, remove_key);
    }
    else
    {
        root->left = remove (root->left, remove_key);
    }

    return balance_tree (root);
}

struct Node *balance_tree (struct Node *root)
{
    if (!(root))
    {
        return nullptr;
    }

    fix_height (root);

    if (b_factor (root) == 2)
    {
        if (b_factor (root->right) < 0)
        {
            root->right = right_rotate (root->right);
        }

        root = left_rotate (root);
    }

    if (b_factor (root) == -2)
    {
        if (b_factor (root->left) > 0)
        {
            root->left = left_rotate (root->left);
        }

        root = right_rotate (root);
    }

    return root;
}


struct Node *find_min (struct Node *root)
{
    if (!(root))
    {
        return nullptr;
    }

    if (root->left)
    {
        return find_min (root->left);
    }

    return root;
}

struct Node *remove_min (struct Node *root)
{
    if (!(root))
    {
        return nullptr;
    }
    if (root->left)
    {
        root->left = remove_min (root->left);
        return balance_tree (root);
    }

    struct Node *old_root = root;

    root = root->right;
    
    free (old_root);

    return balance_tree (root);
}

int node_height (struct Node *node)
{
    return node ? (int)node->height : 0;
}

int b_factor (struct Node *node)
{
    assert (node);

    return node_height (node->right) - node_height (node->left);
}

void fix_height (struct Node *node)
{
    unsigned char h_left  = (unsigned char) node_height (node->left);
    unsigned char h_right = (unsigned char) node_height (node->right);

    node->height = (max(h_left, h_right)) + 1;
}

inline unsigned char max (unsigned char val1, unsigned char val2)
{
    return val1 > val2 ? val1 : val2;
}

struct Node *left_rotate (struct Node *root)
{
    struct Node *new_root = root->right;
    root->right = new_root->left;
    new_root->left = root;

    fix_height (root);
    fix_height (new_root);

    return new_root;
}

struct Node *right_rotate (struct Node *root)
{
    struct Node *new_root = root->left;
    root->left = new_root->right;
    new_root->right = root;

    fix_height (root);
    fix_height (new_root);

    return new_root;
}

void avl_tree_dtor (struct Avl_tree *avl_tree)
{
    if (!(avl_tree))
    {
        return;
    }

    clear_node (avl_tree->root);
    free (avl_tree);
    avl_tree = nullptr;
}

void clear_node (struct Node *node)
{
    if (!(node))
    {
        return;
    }

    clear_node (node->left);
    clear_node (node->right);

    free (node);
    node = nullptr;
}


#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "binary_search_tree.h"

Binary_tree *bin_tree_ctor ()
{
    Binary_tree *tree = (Binary_tree *)calloc (1, sizeof (Binary_tree));
    assert (tree);

    return tree;
}

Node *bin_tree_find (Node *node, Tkey key)
{
    if (!(node))
    {
        return nullptr;
    }

    if (node->key < key)
    {
        return bin_tree_find (node->right, key);
    }
    else if (node->key > key)
    {
        return bin_tree_find (node->left, key);
    }
    else
    {
        return node;
    }
}

Node *bin_tree_min (Node *node)
{
    if (!(node))
    {
        return nullptr;
    }

    if (!(node->left))
    {
        return node;
    }
    else
    {
        return bin_tree_min (node->left);
    }
}

Node *bin_tree_max (Node *node)
{
    if (!(node))
    {
        return nullptr;
    }

    if (!(node->right))
    {
        return node;
    }
    else
    {
        return bin_tree_max (node->right);
    }
}

Node *bin_tree_next (Binary_tree *tree, Tkey key)
{
    assert (tree);
    
    Node *current = tree->root;
    Node *successor = nullptr;

    while (current)
    {
        if (current->key <= key)
        {
            current = current->right;
        }
        else
        {
            successor = current;
            current = current->left;
        }
    }
    
    return successor;
}

Node *bin_tree_prev (Binary_tree *tree, Tkey key)
{
    assert (tree);
    
    Node *current = tree->root;
    Node *successor = nullptr;

    while (current)
    {
        if (current->key >= key)
        {
            current = current->left;
        }
        else
        {
            successor = current;
            current = current->right;
        }
    }
    
    return successor;
}

void bin_tree_insert (Binary_tree *tree, Tkey key)
{
    assert (tree);

    tree->root = bin_tree_insert_node (tree, tree->root, key);

    ++tree->size;
}

Node *bin_tree_insert_node (Binary_tree *tree, Node *node, Tkey key)
{
    if (!(node))
    {
        Node *inserted = (Node *)calloc (1, sizeof (Node));
        inserted->key = key;
        
        node = inserted;

        return node;
    }

    if (key > node->key)
    {
        node->right = bin_tree_insert_node (tree, node->right, key);
    }
    else 
    {
        node->left = bin_tree_insert_node (tree, node->left, key);
    }

    return node;
}

void bin_tree_remove (Binary_tree *tree, Tkey key)
{
    if (!(tree))
    {
        return;
    }

    tree->root = bin_tree_delete_node (tree, tree->root, key);
}

Node *bin_tree_delete_node (Binary_tree *tree, Node *node, Tkey key)
{
    if (!(node))
    {
        return nullptr;
    }

    if (key == node->key)
    {
        if (!(node->left || node->right))
        {
            free (node);
            node = nullptr;
        }
        else if (!(node->left))
        {
            Node *temp = node;
            node = node->right;
            free (temp);
        }
        else if (!(node->right))
        {
            Node *temp = node;
            node = node->left;
            free (temp);
        }
        else 
        {
            Node *new_head = bin_tree_min (node->right);
            node->key = new_head->key;
            node->right = bin_tree_delete_node (tree, node->right, new_head->key);
        }

        --(tree->size);
    }
    else if (key > node->key)
    {
        node->right = bin_tree_delete_node (tree, node->right, key);
    }
    else
    {
        node->left = bin_tree_delete_node (tree, node->left, key);
    }

    return node;
}

void bin_tree_dtor (Binary_tree *tree)
{
    if (!(tree))
    {
        return;
    }

    bin_tree_clear (tree->root);
    free (tree);
    tree = nullptr;
}

void bin_tree_clear (Node *node)
{
    if (!(node))
    {
        return;
    }

    bin_tree_clear (node->left);
    bin_tree_clear (node->right);
    node->left = nullptr;
    node->right = nullptr;
    
    free (node);
    node = nullptr;
}
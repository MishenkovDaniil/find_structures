#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "splay_tree.h"

Splay_tree *splay_ctor ()
{
    Splay_tree *splay_tree = (Splay_tree *)calloc (1, sizeof (Splay_tree));
    assert (splay_tree);

    return splay_tree;
}

Splay_node *splay_node_ctor (Tkey key, Splay_node *left, Splay_node *right, Splay_node *parent)
{
    Splay_node *node = (Splay_node *)calloc (1, sizeof (Splay_node));
    assert (node);

    node->key = key;
    node->left = left;
    node->right = right;
    node->parent = parent;

    set_parent (node);

    return node;
}

void Insert (Splay_tree *splay_tree, Tkey key)
{
    assert (splay_tree);

    Splay_pair splays = Split (splay_tree, key);
    splay_tree->root = splay_node_ctor (key, splays.splay_node_1, splays.splay_node_2, nullptr);
}

void Remove (Splay_tree *splay_tree, Tkey key)
{
    assert (splay_tree);
    
    Splay_node *removed = find (splay_tree, key);
    if (!removed)
        return;

    removed = splay_tree->root;

    Splay_node *left = splay_tree->root->left;
    Splay_node *right = splay_tree->root->right;
    
    if (left) left->parent = nullptr;
    if (right) right->parent = nullptr;

/// else
    splay_tree->root = Merge (splay_tree->root->left, splay_tree->root->right);
    free (removed);
    removed = nullptr;
}

Splay_node *Merge (Splay_node *first, Splay_node *second)
{
    if (!first)
        return second;
    if (!second)
        return first;

    Splay_node *max_node = max (first);

    first = splay_node (max_node);
    first->right = second;
    set_parent (first);

    return first;
}

Splay_node *max (Splay_node *node)
{
    if (!node)
        return nullptr;
    return node->right ? max (node->right) : node;
}

Splay_pair Split (Splay_tree *splay_tree, Tkey key)
{
    assert (splay_tree);
    
    Splay_pair splays = {nullptr, nullptr};

    Splay_node *ge_node = find_ge_node (splay_tree->root, key);
    
    if (ge_node)
    {
        splay_tree->root = splay_node (ge_node);
        splays.splay_node_1 = splay_tree->root->left;
        if (splays.splay_node_1) splays.splay_node_1->parent = nullptr;
        splay_tree->root->left = nullptr;
        splays.splay_node_2 = splay_tree->root;
    }
    else 
    {
        splays.splay_node_1 = splay_tree->root;
    }

    return splays;
}

Splay_node *find (Splay_tree *splay_tree, Tkey key)
{
    assert (splay_tree);

    Splay_node *founded = find_node (splay_tree->root, key);

    if (founded)
    {
        // founded->parent = nullptr;
        splay_tree->root = founded;
    }

    return founded;
}

Splay_node *find_ge_node (Splay_node *node, Tkey key)
{
    if (!node)
        return nullptr;

    if (node->key == key)
    {
        return node;
    }
    else if (key > node->key)
    {
        return find_ge_node (node->right, key);
    }
    else 
    {
        Splay_node *founded = find_ge_node (node->left, key);
        if (founded && founded->key >= key)
        {
            return founded;
        }
        else
        {
            return node;
        }
    }
}

Splay_node *find_node (Splay_node *node, Tkey key)
{
    if (!node)
        return nullptr;
    
    if (key > node->key)
    {
        return find_node (node->right, key);
    }
    else if (key < node->key)
    {
        return find_node (node->left, key);
    }
    
/// if (node->key == key)
    return splay_node (node);                
}

Splay_node *splay_node (Splay_node *node)
{
    if (!(node && node->parent))
        return node;
    
    Splay_node *parent = node->parent;

    if (!grparent (node))
    {
        rotate (node);
        node->parent = nullptr;
        return node;
    }
    else 
    {
        bool is_left_son = is_left (node);
        bool is_left_gr_son = is_left (parent);

        if (is_left_gr_son == is_left_son)
        {
            rotate (parent);                /// zigzig
            rotate (node);
        }
        else
        {
            rotate (node);                  /// zigzag
            rotate (node);
        }

        return splay_node (node);
    }
}

Splay_node *rotate (Splay_node *node)
{
    assert (node && node->parent);

    Splay_node *gr_parent = grparent (node);
    Splay_node *parent = node->parent;
    
    if (gr_parent)
    {
        if (gr_parent->left == parent)
        {
            gr_parent->left = node;
        }
        else 
        {
            gr_parent->right = node;
        }
    }

    if (parent->left == node)
    {
        parent->left = node->right; 
        // update_parent (node->right, parent->left);
        node->right = parent;
        // update_parent (parent, node->right);
    }
    else 
    {
        parent->right = node->left; 
        node->left = parent;
    }

    node->parent = gr_parent;
    set_parent (parent);
    set_parent (node);

    return node;
}   

void set_parent (Splay_node *node)
{
    if (node)
    {
        if (node->left)
            node->left->parent = node;
        if (node->right)
            node->right->parent = node;
    }
}
void splay_dtor (Splay_tree *splay_tree)
{
    if (!splay_tree)
    {
        return;
    }

    splay_clear_node (splay_tree->root);

    free (splay_tree);
    splay_tree = nullptr;
}

void splay_clear_node (Splay_node *node)
{
    if (!node)
        return;

    splay_clear_node (node->left); 
    splay_clear_node (node->right);

    free (node);
    node = nullptr; 
}

Splay_node *parent (Splay_node *node)
{
    return node ? node->parent : nullptr;
}

Splay_node *grparent (Splay_node *node)
{
    return parent (parent(node));
}

bool is_left (Splay_node *node)     /// parent must exist
{
    assert (node && node->parent);

    return node->parent->left == node;
}

void update_parent (Splay_node *child, Splay_node *parent)
{
    if (child)
        child->parent = parent;
}

void splay_dump (struct Splay_tree *splay_tree, const char *log_name)
{
    static int PNG_FILE_NUMBER = 0;

    FILE *tgraph_file = fopen ("splay_graph", "w");
    FILE *treapset_log  = NULL;

    if (!log_name)
    {
        log_name = LOG_NAME;
    }
    if (!(PNG_FILE_NUMBER))
    {
        treapset_log  = fopen (log_name, "w");
    }
    else
    {
        treapset_log  = fopen (log_name, "a");
    }

    fprintf (tgraph_file, "digraph\n{\n\t");

    make_graph_nodes (splay_tree->root, tgraph_file);

    fprintf (tgraph_file, "}");
    fclose (tgraph_file);

    //const int CMD_LEN = 100;
    char cmd[100] = "";

    #ifdef __unix__
    sprintf (cmd, "dot splay_graph -T png -o splay_dot%d.png", PNG_FILE_NUMBER);
    #else
    sprintf (cmd, "Dot splay_graph -T png -o splay_dot%d.png", PNG_FILE_NUMBER);
    #endif

    system ((const char *)cmd);

    fprintf (treapset_log, "<pre>\n<img src = splay_dot%d.png>\n", PNG_FILE_NUMBER++);

    fclose (treapset_log);
}

int make_graph_nodes (struct Splay_node *node, FILE *tgraph_file)
{
    assert (tgraph_file);
    
    static int graph_num = 0;

    int node_num = graph_num;

    if (node == NULL)
    {
        fprintf (tgraph_file, "node_%d [shape = record, style = \"filled\", fillcolor = \"orange\", label = \"{NULL\"];\n\t", graph_num++);
    }
    else
    {
        fprintf (tgraph_file, "node_%d [shape = record, style = \"filled\", fillcolor = \"lightblue\", label = \"{node %p | {parent = %p} | {key | %d} | {L %p | R %p}} \"];\n\t", 
                 graph_num++, node, node->parent, node->key, node->left, node->right);
    }

    if (node->left != NULL)
    {
        int left = make_graph_nodes (node->left, tgraph_file);

        fprintf (tgraph_file, "node_%d->node_%d;\n\t", node_num, left);
    }
    if (node->right != NULL)
    {
        int right = make_graph_nodes (node->right, tgraph_file);

        fprintf (tgraph_file, "node_%d->node_%d;\n\t", node_num, right);
    }

    return node_num;
}
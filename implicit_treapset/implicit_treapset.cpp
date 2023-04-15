#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "implicit_treapset.h"

struct Implicit_treapset *implicit_treapset_ctor ()
{
    struct Implicit_treapset *treapset = (struct Implicit_treapset *)calloc (1, sizeof (struct Implicit_treapset));
    treapset->size = 0;
    treapset->root = nullptr;

    return treapset;
}

struct Implicit_treapnode *implicit_treapnode_ctor (Tprior priority, Tdata data, struct Implicit_treapnode *left, struct Implicit_treapnode *right)
{
    struct Implicit_treapnode *node = (struct Implicit_treapnode *)calloc (1, sizeof (struct Implicit_treapnode));
    assert (node);

    node->data = data;
    node->priority = priority;
    node->left = left;
    node->right = right;
    node->size = node_size (node);

    return node;
}

int node_size (struct Implicit_treapnode *treapnode)
{
    if (!(treapnode))
    {
        return 0;
    }

    return node_size (treapnode->left) + node_size (treapnode->right) + 1;
}

struct TreapNode_pair Split (struct Implicit_treapnode *treapnode, size_t split_pos)
{
    struct TreapNode_pair treaps = {nullptr, nullptr};
    
    if (!(treapnode))
    {
        return treaps;
    }
    
    size_t cur_pos = node_size (treapnode->left) + 1;

    if (cur_pos <= split_pos)
    {
        treaps = Split (treapnode->right, split_pos - treapnode->size);
        treapnode->right = treaps.treapnode_1;
        treaps.treapnode_1 = treapnode;
    }
    else
    {
        treaps = Split (treapnode->left, split_pos);
        treapnode->left = treaps.treapnode_2;
        treaps.treapnode_2 = treapnode;
    }

    return treaps;
}

struct Implicit_treapnode *Merge (struct Implicit_treapnode *treap_1, struct Implicit_treapnode *treap_2)
{
    if (!(treap_1))
    {
        return treap_2;
    }
    if (!(treap_2))
    {
        return treap_1;
    }

    if (treap_1->priority < treap_2->priority)
    {
        treap_2->left = Merge (treap_1, treap_2->left);
        return treap_2;
    }
    else
    {
        treap_1->right = Merge (treap_1->right, treap_2);
        return treap_1;
    }
}

void Insert (struct Implicit_treapset *treapset, size_t position, Tprior priority, Tdata data)
{
    assert (treapset);

    struct Implicit_treapnode *inserted = implicit_treapnode_ctor (priority, data);
    struct TreapNode_pair treaps = Split (treapset->root, position);
    
    treapset->root = Merge (Merge(treaps.treapnode_1, inserted), treaps.treapnode_2);
    refresh_size (treapset->root);
}

bool Erase (struct Implicit_treapset *treapset, size_t position, Tdata data)
{
    assert (treapset);

    struct TreapNode_pair treaps = Split (treapset->root, position);

    if (treaps.treapnode_2 && treaps.treapnode_2->size == position)
    {
        struct Implicit_treapnode *erase_node = treaps.treapnode_2;
        struct TreapNode_pair treaps_2 = Split (treaps.treapnode_2, 1);
        //treaps.treapnode_2 = Merge (treaps.treapnode_2->left, treaps.treapnode_2->right);
        
        free (erase_node);

        treapset->root = Merge (treaps.treapnode_1, treaps_2.treapnode_2);
        refresh_size (treapset->root);
        //treapset->root = Merge (treaps.treapnode_1, treaps.treapnode_2);
        return true;
    }
    
    treapset->root = Merge (treaps.treapnode_1, treaps.treapnode_2);

    return false;
}

void Treapset_dtor (struct Implicit_treapset *treapset)
{
    assert (treapset);

    treapnode_clear (treapset->root);

    free (treapset);
    treapset = nullptr;
}

void treapnode_clear (struct Implicit_treapnode *node)
{
    if (!(node))
    {
        return;
    }

    treapnode_clear (node->left);
    treapnode_clear (node->right);
    free (node);
}

void refresh_size (struct Implicit_treapnode *treapnode)
{
    if (!(treapnode))
    {
        return;
    }

    refresh_size (treapnode->left);
    refresh_size (treapnode->right);

    treapnode->size = node_size(treapnode->left) + node_size(treapnode->right) + 1;
}



void treapset_dump (struct Implicit_treapset *treapset, const char *log_name)
{
    static int PNG_FILE_NUMBER = 0;

    FILE *tgraph_file = fopen ("implicit_treapset_graph", "w");
    FILE *treapset_log  = nullptr;

    if (!(PNG_FILE_NUMBER))
    {
        treapset_log  = fopen (log_name, "w");
    }
    else
    {
        treapset_log  = fopen (log_name, "a");
    }

    fprintf (tgraph_file, "digraph\n{\n\t");

    make_graph_nodes (treapset->root, tgraph_file);

    fprintf (tgraph_file, "}");
    fclose (tgraph_file);

    const int CMD_LEN = 100;
    char cmd[CMD_LEN] = "";

    #ifdef __unix__
    sprintf (cmd, "dot implicit_treapset_graph -T png -o imlicit_treapset_dot%d.png", PNG_FILE_NUMBER);
    #else
    sprintf (cmd, "Dot implicit_treapset_graph -T png -o imlicit_treapset_dot%d.png", PNG_FILE_NUMBER);
    #endif

    system ((const char *)cmd);

    fprintf (treapset_log, "<pre>\n<img src = imlicit_treapset_dot%d.png>\n", PNG_FILE_NUMBER++);

    fclose (treapset_log);
}

int make_graph_nodes (struct Implicit_treapnode *node, FILE *tgraph_file)
{
    assert (tgraph_file);
    
    static int graph_num = 0;

    int node_num = graph_num;

    if (node == nullptr)
    {
        fprintf (tgraph_file, "node_%d [shape = record, style = \"filled\", fillcolor = \"orange\", label = \"{nullptr\"];\n\t", graph_num++);
    }
    else
    {
        fprintf (tgraph_file, "node_%d [shape = record, style = \"filled\", fillcolor = \"lightblue\", label = \"{node %p | {priority = %d} | {size | %d} | {L %p | R %p}} \"];\n\t", 
                 graph_num++, node, node->priority, node->size, node->left, node->right);
    }

    if (node->left != nullptr)
    {
        int left = make_graph_nodes (node->left, tgraph_file);

        fprintf (tgraph_file, "node_%d->node_%d;\n\t", node_num, left);
    }
    if (node->right != nullptr)
    {
        int right = make_graph_nodes (node->right, tgraph_file);

        fprintf (tgraph_file, "node_%d->node_%d;\n\t", node_num, right);
    }

    return node_num;
}
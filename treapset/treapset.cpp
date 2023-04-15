#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "treapset.h"

struct TreapSet *treapset_ctor ()
{
    struct TreapSet *treapset = (struct TreapSet *)calloc (1, sizeof (struct TreapSet));
    treapset->size = 0;

    return treapset;
}

struct TreapNode *treapnode_ctor (Tkey key, Tprior priority, struct TreapNode *left, struct TreapNode *right)
{
    struct TreapNode *node = (struct TreapNode *)calloc (1, sizeof (struct TreapNode));
    assert (node);

    node->key = key;
    node->priority = priority;
    node->left = left;
    node->right = right;

    return node;
}

struct TreapNode_pair Split (struct TreapNode *treapnode, Tkey key)
{
    struct TreapNode_pair treaps = {nullptr, nullptr};
    
    if (!(treapnode))
    {
        return treaps;
    }

    if (treapnode->key < key)
    {
        treaps = Split (treapnode->right, key);
        treapnode->right = treaps.treapnode_1;
        treaps.treapnode_1 = treapnode;
    }
    else
    {
        treaps = Split (treapnode->left, key);
        treapnode->left = treaps.treapnode_2;
        treaps.treapnode_2 = treapnode;
    }

    return treaps;
}

struct TreapNode *Merge (struct TreapNode *treap_1, struct TreapNode *treap_2)
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

void Insert (struct TreapSet *treapset, Tkey key, Tprior priority)
{
    assert (treapset);

    struct TreapNode *inserted = treapnode_ctor (key, priority);
    struct TreapNode_pair treaps = Split (treapset->root, key);
    
    treapset->root = Merge (Merge(treaps.treapnode_1, inserted), treaps.treapnode_2);
}

bool Erase (struct TreapSet *treapset, Tkey key)
{
    assert (treapset);

    struct TreapNode_pair treaps = Split (treapset->root, key);

    if (treaps.treapnode_2 && treaps.treapnode_2->key == key)
    {
        struct TreapNode *erase_node = treaps.treapnode_2;
        treaps.treapnode_2 = Merge (treaps.treapnode_2->left, treaps.treapnode_2->right);

        free (erase_node);
        treapset->root = Merge (treaps.treapnode_1, treaps.treapnode_2);
        return true;
    }
    
    treapset->root = Merge (treaps.treapnode_1, treaps.treapnode_2);

    return false;
}

void Treapset_dtor (struct TreapSet *treapset)
{
    assert (treapset);

    treapnode_clear (treapset->root);

    free (treapset);
    treapset = nullptr;
}

void treapnode_clear (struct TreapNode *node)
{
    if (!(node))
    {
        return;
    }

    treapnode_clear (node->left);
    treapnode_clear (node->right);
    free (node);
}

void treapset_dump (struct TreapSet *treapset, const char *log_name)
{
    static int PNG_FILE_NUMBER = 0;

    FILE *tgraph_file = fopen ("treapset_graph", "w");
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
    sprintf (cmd, "dot tree_graph -T png -o treapset_dot%d.png", PNG_FILE_NUMBER);
    #else
    sprintf (cmd, "Dot tree_graph -T png -o treapset_dot%d.png", PNG_FILE_NUMBER);
    #endif

    system ((const char *)cmd);

    fprintf (treapset_log, "<pre>\n<img src = treapset_dot%d.png>\n", PNG_FILE_NUMBER++);

    fclose (treapset_log);
}

int make_graph_nodes (struct TreapNode *node, FILE *tgraph_file)
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
        fprintf (tgraph_file, "node_%d [shape = record, style = \"filled\", fillcolor = \"lightblue\", label = \"{node %p | {priority = %d} | {key | %d} | {L %p | R %p}} \"];\n\t", 
                 graph_num++, node, node->priority, node->key, node->left, node->right);
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
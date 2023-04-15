#ifndef TREAPSET_H
#define TREAPSET_H

#include <stdio.h>
#include <assert.h>

static const char *LOG_NAME = "treapset_dump.html";

typedef int Tkey;
typedef int Tprior;

struct TreapNode 
{
    TreapNode *left = nullptr;
    TreapNode *right = nullptr;
    Tkey key = 0;
    Tprior priority = 0;
};

struct TreapSet
{
    TreapNode *root = nullptr;
    size_t size = 0;    
};

struct TreapNode_pair 
{
    TreapNode *treapnode_1 = nullptr;
    TreapNode *treapnode_2 = nullptr;
};

struct TreapSet *treapset_ctor ();
struct TreapNode *treapnode_ctor (Tkey key, Tprior priority, struct TreapNode *left = nullptr, struct TreapNode *right = nullptr);
struct TreapNode *Merge (struct TreapNode *treap_1, struct TreapNode *treap_2);
struct TreapNode_pair Split (struct TreapNode *treapnode, Tkey key);
void Insert (struct TreapSet *treapset, Tkey key, Tprior priority);
bool Erase (struct TreapSet *treapset, Tkey key);
void treapnode_clear (struct TreapNode *node);
void Treapset_dtor (struct TreapSet *treapset);
void treapset_dump (struct TreapSet *treapset, const char *log_name = LOG_NAME);
int make_graph_nodes (struct TreapNode *node, FILE *tgraph_file);

#endif /* TREAPSET_H */
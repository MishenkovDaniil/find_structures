#ifndef IMPLICIT_TREAPSET_H
#define IMPLICIT_TREAPSET_H

#include <stdio.h>
#include <assert.h>

static const char *LOG_NAME = "implicit_treapset_dump.html";

typedef int Tdata;
typedef int Tprior;

struct Implicit_treapnode 
{
    Implicit_treapnode *left = nullptr;
    Implicit_treapnode *right = nullptr;
    Tdata data = 0;
    Tprior priority = 0;
    size_t size = 1;
};

struct Implicit_treapset
{
    Implicit_treapnode *root = nullptr;
    size_t size = 0;    
};

struct TreapNode_pair 
{
    Implicit_treapnode *treapnode_1 = nullptr;
    Implicit_treapnode *treapnode_2 = nullptr;
};

/// public
struct Implicit_treapset *implicit_treapset_ctor (void);
void Insert (struct Implicit_treapset *treapset, size_t position, Tprior priority, Tdata data = 0);
bool Erase (struct Implicit_treapset *treapset, size_t pos, Tdata data = 0);
void treapset_dump (struct Implicit_treapset *treapset, const char *log_name = LOG_NAME);
void Treapset_dtor (struct Implicit_treapset *treapset);

/// private
struct Implicit_treapnode *implicit_treapnode_ctor 
        (Tprior priority, Tdata data, struct Implicit_treapnode *left = nullptr, struct Implicit_treapnode *right = nullptr);
struct Implicit_treapnode *Merge (struct Implicit_treapnode *treap_1, struct Implicit_treapnode *treap_2);
struct TreapNode_pair Split (struct Implicit_treapnode *treapnode, size_t split_pos);
void treapnode_clear (struct Implicit_treapnode *node);
void refresh_size (struct Implicit_treapnode *treapnode);
int node_size (struct Implicit_treapnode *treapnode);
int make_graph_nodes (struct Implicit_treapnode *node, FILE *tgraph_file);

#endif /* IMPLICIT_TREAPSET_H */
#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

typedef int Tkey;
static const char *LOG_NAME = "splay_dump.html";

struct Splay_node
{
    Splay_node *left = nullptr;
    Splay_node *right = nullptr;
    Splay_node *parent = nullptr;
    Tkey key = 0;
};  

struct Splay_tree
{
    Splay_node *root = nullptr;
};

struct Splay_pair 
{
    Splay_node *splay_node_1 = nullptr;
    Splay_node *splay_node_2 = nullptr;
};

/// public
Splay_tree *splay_ctor ();
Splay_node *find (Splay_tree *splay_tree, Tkey key);
void Insert (Splay_tree *splay_tree, Tkey key);
void Remove (Splay_tree *splay_tree, Tkey key);
void splay_dtor (Splay_tree *splay_tree);
void splay_dump (struct Splay_tree *splay_tree, const char *log_name = LOG_NAME);

/// private
void set_parent (Splay_node *node);
int make_graph_nodes (struct Splay_node *node, FILE *tgraph_file);
Splay_node *Merge (Splay_node *first, Splay_node *second);
Splay_pair Split (Splay_tree *splay_tree, Tkey key);
Splay_node *find_node (Splay_node *node, Tkey key);
Splay_node *find_ge_node (Splay_node *node, Tkey key);
Splay_node *splay_node_ctor (Tkey key, Splay_node *left, Splay_node *right, Splay_node *parent);
Splay_node *splay_node (Splay_node *node);
Splay_node *rotate (Splay_node *node);
Splay_node *grparent (Splay_node *node);
Splay_node *parent (Splay_node *node);
Splay_node *max (Splay_node *node);
void splay_clear_node (Splay_node *node);
void update_parent (Splay_node *child, Splay_node *parent);
bool is_left (Splay_node *node);     /// parent must exist

#endif /* SPLAY_TREE_H */
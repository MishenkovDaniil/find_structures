#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "binary_search_tree.h"

static const size_t SORT_INSERT_NUM = 100000;
static const size_t INSERT_NUM = 100000;
static const size_t REMOVE_NUM = 50000;
static const size_t CALC_NUM = 5;

void binary_tree_rand_timetest (void);
void binary_tree_sort_timetest (void);
double binary_tree_rand_timetest1 (Binary_tree *tree);
double binary_tree_sort_timetest1 (Binary_tree *tree, int arr[SORT_INSERT_NUM]);
int compare_ints (const void *val1, const void *val2);


int main ()
{
    srand(time(NULL));

    binary_tree_rand_timetest ();
    binary_tree_sort_timetest ();

    return 0;
}

void binary_tree_sort_timetest (void)
{
    int arr[SORT_INSERT_NUM] = {};

    for (size_t i = 0; i < SORT_INSERT_NUM; ++i)
    {
        arr[i] = rand ();
    }

    qsort ((void *)arr, SORT_INSERT_NUM, sizeof (int), compare_ints);

    double time = 0;
    fprintf (stderr, "sok");

    for (size_t calc_iter = 0; calc_iter < CALC_NUM; ++calc_iter)
    {
        Binary_tree *tree = bin_tree_ctor ();
        assert (tree);
        
        time += binary_tree_sort_timetest1 (tree, arr);

        bin_tree_dtor (tree);
    }
        
    double avg_time = time / CALC_NUM;
    printf ("avg bin tree work time on sorted values = %lf ms.\n", avg_time);
}

void binary_tree_rand_timetest (void)
{
    double time = 0;

    for (size_t calc_iter = 0; calc_iter < CALC_NUM; ++calc_iter)
    {
        Binary_tree *tree = bin_tree_ctor ();
        assert (tree);
        
        time += binary_tree_rand_timetest1 (tree);

        bin_tree_dtor (tree);
    }
    
    double avg_time = time / CALC_NUM;
    printf ("avg bin tree work time on random values = %lf ms.\n", avg_time);
}

double binary_tree_sort_timetest1 (Binary_tree *tree, int arr[SORT_INSERT_NUM])
{
    assert (tree);
    
    clock_t strt_time = clock();

    for (size_t insert_iter = 0; insert_iter < SORT_INSERT_NUM; ++insert_iter)
    {
        bin_tree_insert (tree, arr[insert_iter]);
    }
    fprintf (stderr, "ok");
    for (size_t remove_iter = 0; remove_iter < REMOVE_NUM; ++remove_iter)
    {
        int arg = arr[((unsigned int)rand()) % SORT_INSERT_NUM];

        bin_tree_remove (tree, arg);
    }

    clock_t end_time  = clock();

    return (double)(1000 * (end_time - strt_time)) / CLOCKS_PER_SEC;
}

double binary_tree_rand_timetest1 (Binary_tree *tree)
{
    assert (tree);
    
    int arg_arr[INSERT_NUM] = {};

    clock_t strt_time = clock();

    for (size_t insert_iter = 0; insert_iter < INSERT_NUM; ++insert_iter)
    {
        int arg = rand();
        bin_tree_insert (tree, arg);
        arg_arr[insert_iter] = arg;
    }

    for (size_t remove_iter = 0; remove_iter < REMOVE_NUM; ++remove_iter)
    {
        int arg = arg_arr[((unsigned int)rand()) % INSERT_NUM];

        bin_tree_remove (tree, arg);
    }

    clock_t end_time  = clock();

    return (double)(1000 * (end_time - strt_time)) / CLOCKS_PER_SEC;
}

int compare_ints (const void *val1, const void *val2)
{
    int arg1 = *((const int *)val1);    
    int arg2 = *((const int *)val2);

    return (arg1 > arg2) - (arg1 < arg2);    
}
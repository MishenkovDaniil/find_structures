#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "avl_tree.h"

static const size_t STRT_INSERT_NUM  = 100000;
static const size_t DELTA_INSERT_NUM = 100000;
static const size_t MAX_INSERT_NUM   = 1000000;
static const size_t CALC_NUM = 5;

struct Test_values
{
    double insert_time = 0;
    double remove_time = 0;
    double insert_sort_time = 0;
    double remove_sort_time = 0;
};

void avl_tree_timetest (FILE *output);
void add_test_values (Test_values *val1, Test_values *val2);
Test_values avg_test_values (Test_values *values, size_t calc_num);
Test_values avl_tree_timetest1 (Avl_tree *rand_tree, Avl_tree *sort_tree, size_t insert_num, FILE *output);
double avl_tree_insert_test (Avl_tree *tree, size_t insert_num, int *arg_arr);
double avl_tree_remove_test (Avl_tree *tree, size_t remove_num, size_t arr_size, int *arg_arr);
double avl_tree_sort_insert_test (Avl_tree *tree, size_t insert_num, int *arg_arr);
double avl_tree_sort_remove_test (Avl_tree *tree, size_t remove_num, size_t arr_size, int *arg_arr);
int compare_ints (const void *val1, const void *val2);

int main ()
{
    srand (time(NULL));
    
    FILE *output = fopen ("avl_output.txt", "w");

    avl_tree_timetest (output);
    
    fclose (output);

    return 0;
}

void avl_tree_timetest (FILE *output)
{
    for (int size = STRT_INSERT_NUM; size < MAX_INSERT_NUM + 1; size += DELTA_INSERT_NUM)
    {
        Test_values test_values = {};
        
        for (int iter = 0; iter < CALC_NUM; ++iter)
        {
            Avl_tree *rand_tree = avl_tree_ctor ();
            Avl_tree *sort_tree = avl_tree_ctor ();

            Test_values cur_values = avl_tree_timetest1 (rand_tree, sort_tree, size, output);
            
            add_test_values (&test_values, &cur_values);

            avl_tree_dtor (rand_tree);
            avl_tree_dtor (sort_tree);
        }

        test_values = avg_test_values (&test_values, CALC_NUM);

        fprintf (output, "size = %d: \tinsert time \t = %.2lf ms, remove_time \t= %.2lf ms,\n"
                         "\t\t\t\tinsert_sort_time = %.2lf ms, remove_sort_time = %.2lf;\n",
                 size, test_values.insert_time, test_values.remove_time, test_values.insert_sort_time, test_values.remove_sort_time);
        fflush (output);
    }
}

void add_test_values (Test_values *val1, Test_values *val2)
{
    assert (val1 && val2);

    val1->insert_time      += val2->insert_time;
    val1->remove_time      += val2->remove_time;
    val1->insert_sort_time += val2->insert_sort_time;
    val1->remove_sort_time += val2->remove_sort_time;
}

Test_values avg_test_values (Test_values *values, size_t calc_num)
{
    assert (values);

    Test_values avg_values = {};

    avg_values.insert_time      = values->insert_time / calc_num;
    avg_values.remove_time      = values->remove_time / calc_num;
    avg_values.insert_sort_time = values->insert_sort_time / calc_num;
    avg_values.remove_sort_time = values->remove_sort_time / calc_num;

    return avg_values;
}

Test_values avl_tree_timetest1 (Avl_tree *rand_tree, Avl_tree *sort_tree, size_t insert_num, FILE *output)
{
    assert (rand_tree);
    
    Test_values test_values = {};

    int *rand_arg_arr = (int *)calloc (insert_num, sizeof (int));
    int *sort_arg_arr = (int *)calloc (insert_num, sizeof (int));
    assert (rand_arg_arr);
    assert (sort_arg_arr);
    
    for (int i = 0; i < insert_num; ++i)
    {
        sort_arg_arr[i] = rand();
    }

    qsort ((void *)sort_arg_arr, insert_num, sizeof (int), compare_ints);
    
    test_values.insert_time = avl_tree_insert_test (rand_tree, insert_num, rand_arg_arr);
    test_values.remove_time = avl_tree_remove_test (rand_tree, insert_num / 2, insert_num, rand_arg_arr);
    test_values.insert_sort_time = avl_tree_sort_insert_test (sort_tree, insert_num, sort_arg_arr);
    test_values.remove_sort_time = avl_tree_sort_remove_test (sort_tree, insert_num / 2, insert_num, sort_arg_arr);
    
    free (rand_arg_arr);
    free (sort_arg_arr);

    return test_values;
}

double avl_tree_sort_insert_test (Avl_tree *tree, size_t insert_num, int *arg_arr)
{
    assert (tree && arg_arr);

    long time = 0;

    for (size_t insert_iter = 0; insert_iter < insert_num; ++insert_iter)
    {
        clock_t insert_start = clock();
        insert (tree, arg_arr[insert_iter]);
        clock_t insert_end = clock ();

        time += insert_end - insert_start;
    }

    return (double)(1000 * time) / CLOCKS_PER_SEC;
}

double avl_tree_sort_remove_test (Avl_tree *tree, size_t remove_num, size_t arr_size, int *arg_arr)
{
    assert (tree && arg_arr);

    long time = 0;

    for (size_t remove_iter = 0; remove_iter < remove_num; ++remove_iter)
    {
        clock_t remove_start  = clock();
        Remove (tree, arg_arr[remove_iter]);
        clock_t remove_end  = clock();

        time += remove_end - remove_start;
    }


    return (double)(1000 * time) / CLOCKS_PER_SEC;    
}

double avl_tree_insert_test (Avl_tree *tree, size_t insert_num, int *arg_arr)
{
    assert (tree && arg_arr);

    long time = 0;

    for (size_t insert_iter = 0; insert_iter < insert_num; ++insert_iter)
    {
        int arg = rand();
        
        clock_t insert_start = clock();
        insert (tree, arg);
        clock_t insert_end = clock ();

        time += insert_end - insert_start;

        arg_arr[insert_iter] = arg;
    }

    return (double)(1000 * time) / CLOCKS_PER_SEC;
}

double avl_tree_remove_test (Avl_tree *tree, size_t remove_num, size_t arr_size, int *arg_arr)
{
    assert (tree && arg_arr);

    long time = 0;

    for (size_t remove_iter = 0; remove_iter < remove_num; ++remove_iter)
    {
        int arg = arg_arr[((unsigned int)rand()) % arr_size];

        clock_t remove_start  = clock();
        Remove (tree, arg);
        clock_t remove_end  = clock();

        time += remove_end - remove_start;
    }


    return (double)(1000 * time) / CLOCKS_PER_SEC;
}

int compare_ints (const void *val1, const void *val2)
{
    int arg1 = *((const int *)val1);    
    int arg2 = *((const int *)val2);

    return (arg1 > arg2) - (arg1 < arg2);    
}
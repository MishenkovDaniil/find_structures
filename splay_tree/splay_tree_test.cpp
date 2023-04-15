#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "splay_tree.h"

static const size_t STRT_INSERT_NUM  = 100000;
static const size_t DELTA_INSERT_NUM = 100000;
static const size_t MAX_INSERT_NUM   = 1000000;
static const size_t CALC_NUM = 5;

struct Test_values
{
    double insert_time = 0;
    double remove_time = 0;
};

void splay_tree_timetest (FILE *output);
void add_test_values (Test_values *val1, Test_values *val2);
Test_values avg_test_values (Test_values *values, size_t calc_num);
Test_values splay_tree_timetest1 (Splay_tree *splay_tree,size_t insert_num, FILE *output);
double splay_tree_insert_test (Splay_tree *splay_tree, size_t insert_num, int *arg_arr);
double splay_tree_remove_test (Splay_tree *splay_tree, size_t remove_num, size_t arr_size, int *arg_arr);

int main ()
{
    srand (time(NULL));
    
    FILE *output = fopen ("splay_output.txt", "w");

    splay_tree_timetest (output);
    
    fclose (output);

    return 0;
}

void splay_tree_timetest (FILE *output)
{
    for (int i = STRT_INSERT_NUM; i < MAX_INSERT_NUM + 1; i += DELTA_INSERT_NUM)
    {
        Test_values test_values = {};
        
        for (int iter = 0; iter < CALC_NUM; ++iter)
        {
            Splay_tree *splay_tree = splay_ctor ();

            Test_values cur_values = splay_tree_timetest1 (splay_tree, i, output);
            
            add_test_values (&test_values, &cur_values);

            splay_dtor (splay_tree);
        }

        test_values = avg_test_values (&test_values, CALC_NUM);

        fprintf (output, "size = %d: insert time = %.2lf ms, remove_time = %.2lf ms,\n",
                 i, test_values.insert_time, test_values.remove_time);
        fflush (output);
    }
}

Test_values splay_tree_timetest1 (Splay_tree *splay_tree, size_t insert_num, FILE *output)
{
    assert (splay_tree && output);
    
    Test_values test_values = {};

    int *rand_arg_arr = (int *)calloc (insert_num, sizeof (int));
    assert (rand_arg_arr);
    
    test_values.insert_time = splay_tree_insert_test (splay_tree, insert_num, rand_arg_arr);
    test_values.remove_time = splay_tree_remove_test (splay_tree, insert_num / 2, insert_num, rand_arg_arr);
    
    free (rand_arg_arr);

    return test_values;
}

double splay_tree_insert_test (Splay_tree *splay_tree, size_t insert_num, int *arg_arr)
{
    assert (splay_tree && arg_arr);

    long time = 0;

    for (size_t insert_iter = 0; insert_iter < insert_num; ++insert_iter)
    {
        int arg = rand();

        clock_t insert_start = clock();
        Insert (splay_tree, arg);
        clock_t insert_end = clock ();

        time += insert_end - insert_start;
        
        arg_arr[insert_iter] = arg;
    }

    return (double)(1000 * time) / CLOCKS_PER_SEC;
}

double splay_tree_remove_test (Splay_tree *splay_tree, size_t remove_num, size_t arr_size, int *arg_arr)
{
    assert (splay_tree && arg_arr);

    long time = 0;

    for (size_t remove_iter = 0; remove_iter < remove_num; ++remove_iter)
    {
        int arg = arg_arr[((unsigned int)rand()) % arr_size];

        clock_t remove_start  = clock();
        Remove (splay_tree, arg);
        clock_t remove_end  = clock();

        time += remove_end - remove_start;
    }


    return (double)(1000 * time) / CLOCKS_PER_SEC;
}

void add_test_values (Test_values *val1, Test_values *val2)
{
    assert (val1 && val2);

    val1->insert_time      += val2->insert_time;
    val1->remove_time      += val2->remove_time;
}

Test_values avg_test_values (Test_values *values, size_t calc_num)
{
    assert (values);

    Test_values avg_values = {};

    avg_values.insert_time      = values->insert_time / calc_num;
    avg_values.remove_time      = values->remove_time / calc_num;

    return avg_values;
}

#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "random_matrix.h"
#include "defines.h"
#include "matrix_io.h"


static void swap(void *first, void *second, void *dop_memory, size_t size)
{
    memcpy(dop_memory, first, size);
    memcpy(first, second, size);
    memcpy(second, dop_memory, size);
}


matrix_t *rand_matr_create(size_t rows, size_t cols, size_t amount)
{
    srand(time(NULL));
    matrix_t *m = matrix_create(rows, cols);
    if (!m)
        return NULL;

    for (size_t k = 0; k < amount; k++)
        m->data[k / cols][k % cols] = rand() % (MAX_VALUE - MIN_VALUE) + MIN_VALUE;

    char temp[sizeof(mtype_t)];

    for (size_t k = rows * cols - 1; k > 0; k--)
    {
        size_t p = rand() % (k + 1);
        swap(&(m->data[k / cols][k % cols]), &(m->data[p / cols][p % cols]), temp, sizeof(mtype_t));
    }


    return m;
}

matrix_t *rand_matr_create_with_occup(size_t rows, size_t cols, size_t occup_rate)
{
    return rand_matr_create(rows, cols, rows * cols * occup_rate / 100);
}

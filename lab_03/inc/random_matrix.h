#ifndef RANDOM_MATRIX_H
#define RANDOM_MATRIX_H

#include <stdlib.h>
#include "matrix.h"

#define MIN_VALUE 1
#define MAX_VALUE 100

matrix_t *rand_matr_create(size_t rows, size_t cols, size_t elements);
matrix_t *rand_matr_create_with_occup(size_t rows, size_t cols, size_t occup_rate);


#endif  // RANDOM_MATRIX_H

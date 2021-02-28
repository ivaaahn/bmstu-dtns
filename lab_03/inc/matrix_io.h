#ifndef MATRIX_IO_H
#define MATRIX_IO_H

#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"


int read_matrix_size(size_t *cols, size_t *rows);
int fill_matrix(matrix_t *matr);
matrix_t *matrix_init(int *rc);

int read_vector_size(size_t *len);
int fill_vector(matrix_t *vec);
matrix_t *vector_init(int *rc);


void print_matrix(matrix_t *matr);
void print_vector(matrix_t *vec);


int read_rand_matr_size(size_t *cols, size_t *rows, size_t *occup);
int read_rand_vec_size(size_t *len, size_t *occup);

void print_empty_matrix(size_t rows, size_t cols);


#endif  // MATRIX_IO_H
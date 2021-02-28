#ifndef MATRIX_H
#define MATRIX_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define MTYPE_SPEC "ld"
typedef long mtype_t;


typedef struct
{
    size_t rows;
    size_t cols;
    mtype_t **data;

} matrix_t;

typedef struct
{
    size_t rows;
    size_t cols;
    size_t count;
    
    mtype_t *values;
    size_t *col_indexes;
    size_t *start_rows;

} sp_matrix_t;


matrix_t *matrix_create(size_t rows, size_t cols);
sp_matrix_t *sp_matrix_create(size_t rows, size_t cols, size_t count);

sp_matrix_t *m_to_sparse(matrix_t *m);
matrix_t *sp_to_matrix(sp_matrix_t *sparse);

void del_matrix(matrix_t *matrix);  
void del_sp_matrix(sp_matrix_t *matrix);

matrix_t *multiply(const matrix_t *left, const matrix_t *right, clock_t *time);
sp_matrix_t *sp_multiply(const sp_matrix_t *vec, const sp_matrix_t *m, clock_t *time);

size_t get_matrix_size(size_t rows, size_t cols);
size_t get_sp_matrix_size(size_t rows, size_t count);

#endif  // MATRIX_H
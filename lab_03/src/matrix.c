#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "matrix.h"
#include "defines.h"


matrix_t *matrix_create(size_t rows, size_t cols)
{
    matrix_t *m = malloc(sizeof(matrix_t));

    if (!m) {
        return NULL;
    }

    
    m->cols = cols;
    m->rows = rows;

    m->data = calloc(rows * sizeof(mtype_t *) + rows * cols * sizeof(mtype_t), 1);
    if (!(m->data)) {
        free(m);
        return NULL;
    }

    for (size_t i = 0; i < rows; i++) {
        m->data[i] = (mtype_t *)(m->data + rows) + cols * i;
    }

    return m;
}

void del_matrix(matrix_t *matr)
{
    if (!matr)
        return;

    free(matr->data);
    
    free(matr);
}

sp_matrix_t *sp_matrix_create(size_t rows, size_t cols, size_t count)
{
    sp_matrix_t *m = malloc(sizeof(sp_matrix_t));
    if (!m)
        return NULL;

    m->cols = cols;
    m->rows = rows;
    m->count = count;

    m->values = calloc(sizeof(mtype_t), count);
    if (!(m->values))
    {
        free(m);
        return NULL;
    }

    m->col_indexes = calloc(sizeof(mtype_t), count);
    if ((!m->col_indexes))
    {
        free(m->values);
        free(m);
        return NULL;
    }

    m->start_rows = calloc(sizeof(size_t), rows + 1);
    if (!(m->start_rows))
    {
        free(m->col_indexes);
        free(m->values);
        free(m);
        return NULL;
    }
    m->start_rows[rows] = count;

    return m;
}

void del_sp_matrix(sp_matrix_t *matr)
{
    if (!matr)
        return;

    free(matr->values);
    free(matr->col_indexes);
    free(matr->start_rows);

    free(matr);
}

sp_matrix_t *m_to_sparse(matrix_t *matr)
{
    size_t count = 0;
    for (size_t i = 0; i < matr->rows; i++)
        for (size_t j = 0; j < matr->cols; j++)
            if (matr->data[i][j])
                count++;

    sp_matrix_t *sparse = sp_matrix_create(matr->rows, matr->cols, count);
    if (!sparse)
        return NULL;

    size_t k = 0;
    for (size_t i = 0; i < matr->rows; i++)
    {
        sparse->start_rows[i] = k;

        for (size_t j = 0; j < matr->cols; j++)
        {
            if (matr->data[i][j])
            {
                sparse->values[k] = matr->data[i][j];
                sparse->col_indexes[k] = j;
                k++;
            }
        }
    }


    return sparse;
}

matrix_t *sp_to_matrix(sp_matrix_t *sp)
{
    matrix_t *m = matrix_create(sp->rows, sp->cols);
    if (!m)
        return NULL;

    for (size_t i = 0; i < sp->rows; i++)
    {
        for (size_t k = sp->start_rows[i]; k < sp->start_rows[i + 1]; k++)
            m->data[i][sp->col_indexes[k]] = sp->values[k];
    }


    return m;
}


matrix_t *multiply(const matrix_t *vec, const matrix_t *matr, clock_t *time)
{

    matrix_t *ans;
    if (!(ans = matrix_create(1, matr->cols)))
        return NULL;

    clock_t start = clock();
    
    for (size_t j_vec = 0; j_vec < vec->cols; j_vec++)
    {
        for (size_t j_matr = 0; j_matr < matr->cols; j_matr++)
        {
            ans->data[0][j_matr] += vec->data[0][j_vec] * matr->data[j_vec][j_matr];
        }
    }

    *time = clock() - start;


    return ans;
}


sp_matrix_t *sp_multiply(const sp_matrix_t *vec, const sp_matrix_t *m, clock_t *time)
{
 
    clock_t start = clock();


    matrix_t *ans_vec = matrix_create(1, m->cols);
    if (!ans_vec)
        return NULL;

    ssize_t *ip = malloc(sizeof(size_t) * vec->cols);
    if (!ip)
    {
        del_matrix(ans_vec);
        return NULL;
    }

    for (size_t k = 0; k < vec->cols; k++)
        ip[k] = -1;

    for (size_t k = 0; k < vec->count; k++)
        ip[vec->col_indexes[k]] = k;

   
    size_t target_j_vec;
    
    for (size_t i_matr = 0; i_matr < m->rows; i_matr++)
    {
        target_j_vec = i_matr;  
        if (ip[target_j_vec] != -1)
            for (size_t j_matr = m->start_rows[i_matr]; j_matr < m->start_rows[i_matr + 1]; j_matr++)
                ans_vec->data[0][m->col_indexes[j_matr]] += vec->values[ip[target_j_vec]] * m->values[j_matr];
    }

    sp_matrix_t *sp_vector_ans = m_to_sparse(ans_vec);
    if (!sp_vector_ans)
    {
        del_matrix(ans_vec);
        free(ip);
        return NULL;
    }

    *time = clock() - start;


    free(ip);
    del_matrix(ans_vec);

    return sp_vector_ans;
}



size_t get_matrix_size(size_t rows, size_t cols)
{
    return sizeof(matrix_t) + sizeof(mtype_t) * cols * rows;
}

size_t get_sp_matrix_size(size_t rows, size_t count)
{
    return (sizeof(mtype_t) + sizeof(size_t)) * count + sizeof(size_t) * (rows + 1);
}
#include <stdio.h>
#include <stdlib.h>

#include "matrix.h"
#include "defines.h"
#include "utils.h"


int read_vector_size(size_t *len)
{
    long long tmp_len;


    printf("Введите длину вектора: ");

    if (scanf("%lld", &tmp_len) != 1 || tmp_len <= 0)
    {
        wait_and_clear();
        return ERR_INPUT;
    }

    *len = (size_t)tmp_len;


    return OK;
}

int fill_vector(matrix_t *vec)
{
    long long num_of_elements;
    long long curr_index;
    mtype_t curr_elem;

    
    printf("Введите количество элементов, которые Вы хотите ввести: ");

    if (scanf("%lld", &num_of_elements) != 1)
    {
        wait_and_clear();
        return ERR_INPUT;
    }

    if (num_of_elements < 0 || (size_t)num_of_elements > vec->cols)
    {
        wait_and_clear();
        return ERR_INPUT;
    }

    for (size_t i = 0; i < (size_t)num_of_elements; i++)
    {
        printf("Введите элемент (%zu) в формате \"номер_элемента=элемент\": ", i + 1);

        if (scanf("%lld=%" MTYPE_SPEC "", &curr_index, &curr_elem) != 2)
        {
            wait_and_clear();
            return ERR_INPUT;
        }

        if (curr_index < 1LL || (size_t)curr_index > vec->cols)
        {
            wait_and_clear();
            return ERR_INPUT;
        }

        vec->data[0][curr_index - 1] = curr_elem;
    }


    return OK;
}

matrix_t *vector_init(int *rc)
{    
    size_t length;
    if ((*rc = read_vector_size(&length)) != OK)
        return NULL;

    matrix_t *vec; 
    if (!(vec = matrix_create(1, length)))
    {
        *rc = ERR_ALLOCATE;
        return NULL;
    }

    if ((*rc = fill_vector(vec)) != OK)
    {
        del_matrix(vec);
        return NULL;
    }

    *rc = OK;
    return vec;
}



int read_matrix_size(size_t *cols, size_t *rows)
{
    long long tmp_rows, tmp_cols;


    printf("Введите кол-во строк, столбцов через пробел: ");

    if (scanf("%lld%lld", &tmp_rows, &tmp_cols) != 2)
    {
        wait_and_clear();
        return ERR_INPUT;
    }

    if (tmp_rows <= 0 || tmp_cols <= 0)
    {
        wait_and_clear();
        return ERR_INPUT;
    }

    *cols = (size_t)tmp_cols;
    *rows = (size_t)tmp_rows;


    return OK;
}

int read_rand_matr_size(size_t *cols, size_t *rows, size_t *occup)
{
    long long tmp_rows, tmp_cols, tmp_occup;

    printf("Введите размеры матрицы и процент заполненности через пробел: ");

    if (scanf("%lld%lld%lld", &tmp_rows, &tmp_cols, &tmp_occup) != 3)
    {
        wait_and_clear();
        return ERR_INPUT;
    }

    if (tmp_rows <= 0 || tmp_cols <= 0 || tmp_occup > 100 || tmp_occup < 0)
    {
        wait_and_clear();
        return ERR_INPUT;
    }


    *cols = (size_t)tmp_cols;
    *rows = (size_t)tmp_rows;
    *occup = (size_t)tmp_occup;


    return OK;
}


int read_rand_vec_size(size_t *len, size_t *occup)
{
    long long tmp_len, temp_occup;

    printf("Введите размер вектора и процент заполнености через пробел: ");

    if (scanf("%lld%lld", &tmp_len, &temp_occup) != 2)
    {
        wait_and_clear();
        return ERR_INPUT;
    }

    if (tmp_len <= 0 || temp_occup > 100 || temp_occup < 0)
    {
        wait_and_clear();
        return ERR_INPUT;
    }


    *len = (size_t)tmp_len;
    *occup = (size_t)temp_occup;

    return OK;
}



int fill_matrix(matrix_t *matr)
{
    long long num_of_elements;
    long long curr_row, curr_col;
    mtype_t curr_elem;

    printf("Введите количество элементов, которые Вы хотите ввести: ");

    if (scanf("%lld", &num_of_elements) != 1)
    {
        wait_and_clear();
        return ERR_INPUT;
    }

    if (num_of_elements < 0 || (size_t)num_of_elements > matr->rows * matr->cols)
    {
        wait_and_clear();
        return ERR_INPUT;
    }

    for (size_t i = 0; i < (size_t)num_of_elements; i++)
    {
        printf("Введите элемент (%zu) в формате \"номер_строки;номер_столбца=элемент\": ", i + 1);

        if (scanf("%lld;%lld=%" MTYPE_SPEC "", &curr_row, &curr_col, &curr_elem) != 3)
        {
            wait_and_clear();
            return ERR_INPUT;
        }

        if (curr_row < 1LL || (size_t)curr_row > matr->rows)
        {
            wait_and_clear();
            return ERR_INPUT;
        }

        if (curr_col < 1LL || (size_t)curr_col > matr->cols)
        {
            wait_and_clear();
            return ERR_INPUT;
        }


        matr->data[curr_row - 1][curr_col - 1] = curr_elem;
    }

    return OK;
}


matrix_t *matrix_init(int *rc)
{
    size_t rows, cols;
    if ((*rc = read_matrix_size(&cols, &rows) != OK))
        return NULL;

    matrix_t *matr;
    if (!(matr = matrix_create(rows, cols)))
    {
        *rc = ERR_ALLOCATE;
        return NULL;
    }

    if ((*rc = fill_matrix(matr)) != OK)
    {
        del_matrix(matr);
        return NULL;
    }
    
    *rc = OK;
    return matr;
}


void print_matrix(matrix_t *matr)
{
    printf("Строки: %zu, Столбцы: %zu\n"ANSI_COLOR_GREEN, matr->rows, matr->cols);

    for (size_t i = 0; i < matr->rows; i++)
    {
        for (size_t j = 0; j < matr->cols; j++)
            printf("%6"MTYPE_SPEC" ", matr->data[i][j]);

        printf("\n");
    }
    printf("\n\n"ANSI_COLOR_RESET);
}

void print_vector(matrix_t *vec)
{
    printf("Длина: %zu\n"ANSI_COLOR_GREEN, vec->cols);

    for (size_t j = 0; j < vec->cols; j++)
        printf("%6"MTYPE_SPEC" ", vec->data[0][j]);
    
    printf("\n\n"ANSI_COLOR_RESET);

}


void print_empty_matrix(size_t rows, size_t cols)
{
    printf("Строки: %zu, Столбцы: %zu\n", rows, cols);

    for (size_t i = 0; i < rows; i++)
    {
        for (size_t j = 0; j < cols; j++)
            printf("%" MTYPE_SPEC "\t", (mtype_t)0);

        printf("\n");
    }
}

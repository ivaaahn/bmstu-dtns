#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "matrix.h"
#include "handler.h"
#include "matrix_io.h"
#include "random_matrix.h"
#include "defines.h"
#include "utils.h"


matrix_t *full_matrix = NULL;
sp_matrix_t *sparse_matrix = NULL;

matrix_t *full_vector = NULL;
sp_matrix_t *sparse_vector = NULL;


int matrix_init_wrapper()
{
    int rc;


    del_matrix(full_matrix);

    if (!(full_matrix = matrix_init(&rc)))
        return rc;

    del_sp_matrix(sparse_matrix);

    sparse_matrix = m_to_sparse(full_matrix);
    if (!sparse_matrix)
    {
        del_matrix(full_matrix);
        return ERR_ALLOCATE;
    }
    
    return OK;
}

int vector_init_wrapper()
{
    int rc;


    del_matrix(full_vector);

    if (!(full_vector = vector_init(&rc)))
        return rc;


    del_sp_matrix(sparse_vector);

    sparse_vector = m_to_sparse(full_vector);
    if (!sparse_vector)
    {
        del_matrix(full_vector);
        return ERR_ALLOCATE;
    }

    return OK;
}

int print_matrix_wrapper()
{
    if (full_matrix)
        print_matrix(full_matrix);
    else
        printf("Матрица еще не заполнена. ");


    return OK;
}

int print_vector_wrapper()
{
    if (full_vector)
        print_vector(full_vector);
    else
        printf("Вектор еще не заполнен. ");


    return OK;
}


int rand_matr_create_wrapper()
{
    int rc; 
    size_t rows, cols, occup;

    if ((rc = read_rand_matr_size(&cols, &rows, &occup)) != OK)
        return rc;

    del_matrix(full_matrix);

    full_matrix = rand_matr_create_with_occup(rows, cols, occup);
    if (!full_matrix)
        return ERR_ALLOCATE;
    

    del_sp_matrix(sparse_matrix);

    sparse_matrix = m_to_sparse(full_matrix);
    if (!sparse_matrix)
    {
        del_matrix(full_matrix);
        return ERR_ALLOCATE;
    }

    wait_and_clear();
    return OK;
}

int rand_vec_create_wrapper()
{
    int rc;
    size_t len, occup;

    if ((rc = read_rand_vec_size(&len, &occup)) != OK)
        return rc;

    del_matrix(full_vector);
    
    full_vector = rand_matr_create_with_occup(1, len, occup);
    if (!full_vector)
        return ERR_ALLOCATE;

    
    del_sp_matrix(sparse_vector);
    sparse_vector = m_to_sparse(full_vector);
    if (!sparse_vector)
    {
        del_matrix(full_vector);
        return ERR_ALLOCATE;
    }

    wait_and_clear();
    return OK;
}


int mult_wrapper()
{
    if (!full_matrix)
    {
        printf("Матрица еще не заполнена. ");
        return OK;
    }

    if (!full_vector)
    {
        printf("Вектор еще не заполнен. ");
        return OK;
    }

    if (full_vector->cols != full_matrix->rows)
    {
        printf("Матрицу размера %zuX%zu нельзя умножить на вектор длины %zu. ",
            full_matrix->rows, full_matrix->cols, full_vector->cols);

        return ERR_DIMENSION;
    }

    clock_t time_full = 0, time_sparse = 0;

    matrix_t *res_vector = multiply(full_vector, full_matrix, &time_full);
    if (!res_vector)
        return ERR_ALLOCATE;

    sp_matrix_t *res_sp_vector = sp_multiply(sparse_vector, sparse_matrix, &time_sparse);
    if (!res_sp_vector)
    {
        del_matrix(res_vector);
        return ERR_ALLOCATE; 
    }

    matrix_t *temp = sp_to_matrix(res_sp_vector);
    if (!temp)
    {
        del_matrix(res_vector);
        del_sp_matrix(res_sp_vector);
        return ERR_ALLOCATE;
    }

    printf(ANSI_COLOR_YELLOW"### Исходный вектор: \n"ANSI_COLOR_RESET);
    print_vector(full_vector);

    printf(ANSI_COLOR_YELLOW"### Исходная матрица: \n"ANSI_COLOR_RESET);
    print_matrix(full_matrix);
    
    printf(ANSI_COLOR_YELLOW"### Результат умножения полных вектора и матрицы.\n"ANSI_COLOR_RESET);
    print_vector(res_vector);
    printf(ANSI_COLOR_YELLOW"\n ### Результат умножения разреженных вектора и матрицы.\n"ANSI_COLOR_RESET);
    print_vector(temp);
    printf("\n");

    del_matrix(temp);
    del_matrix(res_vector);
    del_sp_matrix(res_sp_vector);

  return OK;
}



int mul_analize()
{
    const size_t repeats = 100;
    printf(ANSI_COLOR_BLUE"#### Анализ умножения вектора-строки на матрицу ####\n"ANSI_COLOR_YELLOW);
    printf("Для каждого измерения взято среднее значение\n");
    printf("по времени для %4zu разных случайных пар матрица-", repeats);
    printf("вектор.\n\nВремя работы указано ");
    printf("в микросекундах.\nОбъём матриц указан в байтах.\nЗаполненность ");
    printf("указана в процентах\n\n"ANSI_COLOR_RESET);

    printf(ANSI_COLOR_GREEN"| Размер  |Запол-|       Время умнож.  |    Объём матриц     |\n");
    printf("| матриц  |  сть |    полных| разрежен.|    полных| разрежен.|\n");
    printf("|---------|------|----------|----------|----------|----------|\n"ANSI_COLOR_RESET);

    for (size_t size = 16; size < 3025; size *= 2)
    {
        for (size_t occup = 0; occup <= 100; occup += 10)
        {
            clock_t time_1 = 0, time_2 = 0;
            for (size_t rep = 0; rep < repeats; rep++)
            {
                matrix_t *matr = rand_matr_create_with_occup(size, size, occup);
                matrix_t *vec = rand_matr_create_with_occup(1, size, occup);
                
                sp_matrix_t *sp_matr = m_to_sparse(matr);
                sp_matrix_t *sp_vec = m_to_sparse(vec);
                
                clock_t cur_1 = 0;
                clock_t cur_2 = 0;

                del_matrix(multiply(vec, matr, &cur_1));
                del_sp_matrix(sp_multiply(sp_vec, sp_matr, &cur_2));
    
                time_1 += cur_1;
                time_2 += cur_2;

                del_matrix(matr);
                del_matrix(vec);
                del_sp_matrix(sp_matr);
                del_sp_matrix(sp_vec);
            }

            printf("|%4zuX%-4zu|  %3zu |%10ld|%10ld|%10zu|%10zu|\n", size, size, occup,
                    time_1 / repeats, time_2 / repeats,
                    get_matrix_size(size, size) + get_matrix_size(1, size),
                    get_sp_matrix_size(size, size * size * occup / 100) +
                        get_sp_matrix_size(size, size * occup / 100));
        }
    }


    return OK;
}

int exit_handler()
{
    printf("Приложение успешно закрыто!\n");
    exit(0);

    del_matrix(full_matrix);
    del_matrix(full_vector);
    del_sp_matrix(sparse_matrix);
    del_sp_matrix(sparse_vector);


    full_vector = NULL;
    full_matrix = NULL;
    sparse_matrix = NULL;
    sparse_vector = NULL;

    return OK;
}

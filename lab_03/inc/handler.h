#ifndef HANDLER_H
#define HANDLER_H

#include <stdlib.h>



int matrix_init_wrapper();
int vector_init_wrapper();


int print_matrix_wrapper();
int print_vector_wrapper();


int rand_matr_create_wrapper();
int rand_vec_create_wrapper();

int mult_wrapper();

size_t get_matrix_size(size_t rows, size_t cols);
size_t get_sp_matrix_size(size_t rows, size_t count);

int mul_analize();
int clear_input();
int wait_and_clear();
int exit_handler();

int mul_analize2();
int wait_and_clear();


#endif  // HANDLER_H
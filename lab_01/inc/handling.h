/**
*   \file handling.c 
*   \brief Обработка чисел
*   \author Ивахненко Дмитрий
*   \date 21.09.2020
*
*   Данный файл содержит функции, предназначенные для обработки (произведения вычислений) длинных чисел.
*/

#ifndef HANDLING_H
#define HANDLING_H

#include "data_structures.h"


/**
    \brief Выполняет деление длинных вещественных чисел.
    \param[in] a делимое.
    \param[in] b делитель.
    \param[out] ans частное.
*/
void divide(lreal_t *a, lreal_t *b, lreal_t *ans);

/**
    \brief Выполняет сложение мантисс.
    \param[in] a первое слагаемое.
    \param[in] b второе слагаемое.
    \param[out] ans сумма.
*/
void add(mant_t *a, mant_t *b, mant_t *ans);

/**
    \brief Выполняет вычитание мантисс.
    \param[in] a уменьшаемое.
    \param[in] b вычитаемое.
    \param[out] ans разность.
*/
void substr(mant_t *a, mant_t *b, mant_t *ans);

/**
    \brief Shift to left (Выполняет сдвиг влево на 1 цифру)
    \param[in, out] a уменьшаемое.
*/
void shtor(mant_t *a);

/**
    \brief Shift to right (Выполняет сдвиг влево на 1 цифру)
    \param[in, out] a уменьшаемое.
*/
void shtol(mant_t *a);


/**
    \brief Инкрементирует длинное вещественное число
    \param[in, out] a число, которое надо инкрементировать
*/
void inc(lreal_t *a);

/**
    \brief Выполняет округления длинного вещественного числа
    \param[in, out] a число, которое надо округлить
*/
void rnd(lreal_t *a);

/**
    \brief Сравнивает два длинных целых числа
    \param[in] a первое число для сравнения 
    \param[in] b второе число для сравнения
    \return положительное число, если a > b, отрицательное число, если a < b, иначе - 0.

*/
int cmp(const mant_t *a, const mant_t *b);


#endif // HANDLIG_H
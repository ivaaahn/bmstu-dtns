#ifndef DS_H
#define DS_H

#include <inttypes.h>
#include "defines.h"

/**
 \typedef строка, длиной LEN_INPUT_LONG (33) содержащая длинное целое число
*/
typedef char lstr_t[LEN_INPUT_LONG];

/**
 \typedef строка, длиной LEN_INPUT_LREAL (41) содержащая длинное вещественное число
*/
typedef char lrstr_t[LEN_INPUT_LREAL];


/**
 * Структура, представляющая мантиссу 
*/
typedef struct 
{
    int8_t sign; /**< знак мантиссы */
    int8_t num[MANT_LEN + 1]; /**< значение мантиссы */
} mant_t;


/**
 * Структура, представляющая длинное вещественное число 
*/
typedef struct
{
    mant_t mantissa; /**< мантисса */
    int32_t exp; /**< экспонента*/
} lreal_t;

/**
 * Перечисление для интерпретации логических значений ИСТИНА ЛОЖЬ
*/
typedef enum
{
    FALSE,
    TRUE
} bool_t;


#endif // DS_H
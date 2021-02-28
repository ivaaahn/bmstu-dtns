#include <string.h>
#include <ctype.h>
#include <inttypes.h>
#include <stdlib.h>

#include "parsing.h"
#include "defines.h"
#include "data_structures.h"

int parse_sign(const lrstr_t e_num, mant_t *p_num)
{
    if (e_num[0] == '-')
        p_num->sign = NEGATIVE;
    else
        p_num->sign = POSITIVE;    
    
    if (e_num[0] == '+' || e_num[0] == '-')
        return OK;

    return NOSIGN;
}

int parse_dot(const lrstr_t e_num, uint8_t *pos)
{
    bool_t dot_check = FALSE;
    uint8_t l_index = 0;

    while (e_num[l_index] && !(e_num[l_index] == 'e' || e_num[l_index] == 'E')) 
    {
        if (e_num[l_index] == '.') 
        {
            if (!dot_check) 
            {
                dot_check = TRUE;
                *pos = l_index;
            }
            else
                return ERR_DOT_PARSE;
        }

        l_index++;
    }

    if (!dot_check)
        *pos = l_index;

    return OK;
}

int parse_significant_digit(const char *e_num, uint8_t *pos)
{
    uint8_t l_index = 0;

    while (e_num[l_index] && !(e_num[l_index] == 'e' || e_num[l_index] == 'E')) 
    {
        if (isdigit((int)e_num[l_index]) && e_num[l_index] - '0' > 0) 
        {
            *pos = l_index;
            return OK;
        } 

        l_index++;
    }
    
    *pos = l_index;
    return ERR_S_DIGIT_PARSE;
}


int parse_integer(const lstr_t e_num, lreal_t *p_num)
{
    uint8_t l_index = 0, new_index = 1;
    bool_t check_sign_digit = FALSE;

    if (parse_sign(e_num, &(p_num->mantissa)) == OK)
        l_index++;

    while (e_num[l_index]) 
    {
        if (isdigit((int)e_num[l_index]))
        {
            if (!check_sign_digit && e_num[l_index] > '0')
                check_sign_digit = TRUE;

            if (check_sign_digit)
            {
                p_num->mantissa.num[new_index++] = e_num[l_index] - '0';
                p_num->exp++; 
            }
        }
        else 
            return ERR_INTEGER_PARSE;

        l_index++;
    }

    if (!check_sign_digit)
        return NUMBER_IS_ZERO;

    return OK;
}

int parse_exp(const char *exp, lreal_t *p_num)
{
    uint8_t l_index = 0;

    if (!exp[l_index])
        return ERR_EXP_PARSE;    

    if (exp[l_index] == '+' || exp[l_index] == '-')
        l_index++;

    if (strlen(exp) - l_index > EXP_LEN)    
        return ERR_EXP_OVERFLOW;

    while (exp[l_index])
    {
        if (!isdigit((int)exp[l_index]))
            return ERR_EXP_PARSE;
        
        l_index++;
    }

    if (strlen(exp) == 1 && !isdigit((int)*exp))
        return ERR_EXP_PARSE;

    p_num->exp += atoi(exp);

    return OK;
}

bool_t is_correct_mantissa(const char * const str_start, const char * const str_end)
{
    const char *curr = str_start;

    if (str_start == str_end && !isdigit((int)*str_end))
        return FALSE;

    while(curr < str_end)
    {
        if (!(isdigit((int)*curr) || *curr == '.'))
            return FALSE;
        curr++;
    }

    return TRUE;
}


int parse_real(const lrstr_t e_num, lreal_t *p_num)
{
    int rc;
    uint8_t dot_pos, first_s_digit_pos;
    uint8_t l_index = 0, new_index = 1;
    bool_t check_mantissa = FALSE;


    if (parse_sign(e_num, &(p_num->mantissa)) == OK)
        l_index++;

    if ((rc = parse_dot(e_num, &dot_pos)) != OK)
        return rc;
  
    if ((rc = parse_significant_digit(e_num, &first_s_digit_pos)) == OK)
        check_mantissa = TRUE;

    if (!is_correct_mantissa(e_num + (char)l_index, e_num + (char)first_s_digit_pos))
        return ERR_REAL_PARSE;

    if (!check_mantissa)
    {
        if ((first_s_digit_pos - l_index == 1) && (e_num[l_index] == '.'))
            return ERR_REAL_PARSE;
    
        return NUMBER_IS_ZERO;
    }

    if ((p_num->exp = dot_pos - first_s_digit_pos) < 0)
        p_num->exp++;

    l_index = first_s_digit_pos;

    while (e_num[l_index])
    {
        if(isdigit((int)e_num[l_index]))
            p_num->mantissa.num[new_index++] = e_num[l_index] - '0';

        else if (isalpha((int)e_num[l_index]))
        {
            if (tolower((int)e_num[l_index]) == 'e')
                break;
            else
                return ERR_REAL_PARSE;
        }

        else if(e_num[l_index] != '.')
            return ERR_REAL_PARSE;

        l_index++;
    }

    if (new_index > MANT_LEN + 1)
        return ERR_MANTISSA_OVERFLOW;

    if (e_num[l_index])
    {
        if ((rc = parse_exp(e_num + l_index + 1, p_num)) != OK)
            return rc;
    }

    return OK;
}